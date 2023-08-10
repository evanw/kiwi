import { Field, Definition, Schema } from "./schema";
import { quote, error } from "./util";

function popTrailingNewline(lines: string[]): void {
  if (lines[lines.length - 1] === '') {
    lines.pop();
  }
}

function skewDefaultValueForField(definitions: { [name: string]: Definition }, field: Field): string {
  if (field.isArray) {
    return 'null';
  }

  switch (field.type) {
    case 'bool': return 'false';
    case 'byte':
    case 'int':
    case 'uint': return '0';
    case 'float': return '0.0';
    case 'string': return 'null';
    case 'int64':
    case 'uint64': return 'BigInt.new(0)';
  }

  let def = definitions[field.type!];

  if (def.kind === 'ENUM') {
    if (def.fields.length > 0) {
      return '.' + def.fields[0].name;
    }
    return '0 as ' + field.type;
  }

  return 'null';
}

function skewTypeForField(field: Field): string | null {
  let type: string | null;

  switch (field.type) {
    case 'bool': type = 'bool'; break;
    case 'byte':
    case 'int':
    case 'uint': type = 'int'; break;
    case 'float': type = 'double'; break;
    case 'string': type = 'string'; break;
    case 'int64':
    case 'uint64': type = 'BigInt'; break;
    default: type = field.type; break;
  }

  if (field.isArray) {
    type = 'List<' + type + '>';
  }

  return type;
}

export function compileSchemaSkew(schema: Schema): string {
  let definitions: { [name: string]: Definition } = {};
  let indent = '';
  let lines: string[] = [];

  if (schema.package !== null) {
    lines.push('namespace ' + schema.package + ' {');
    indent += '  ';
  }

  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    definitions[definition.name] = definition;
  }

  lines.push(indent + 'class BinarySchema {');
  lines.push(indent + '  var _schema = Kiwi.BinarySchema.new');

  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    if (definition.kind === 'MESSAGE') {
      lines.push(indent + '  var _index' + definition.name + ' = 0');
    }
  }

  lines.push('');
  lines.push(indent + '  def parse(bytes Uint8Array) {');
  lines.push(indent + '    _schema.parse(Kiwi.ByteBuffer.new(bytes))');

  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    if (definition.kind === 'MESSAGE') {
      lines.push(indent + '    _index' + definition.name + ' = _schema.findDefinition("' + definition.name + '")');
    }
  }

  lines.push(indent + '  }');

  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    if (definition.kind === 'MESSAGE') {
      lines.push('');
      lines.push(indent + '  def skip' + definition.name + 'Field(bb Kiwi.ByteBuffer, id int) {');
      lines.push(indent + '    _schema.skipField(bb, _index' + definition.name + ', id)');
      lines.push(indent + '  }');
    }
  }

  lines.push(indent + '}');
  lines.push('');

  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];

    switch (definition.kind) {
      case 'ENUM': {
        let encode: any = {};
        let decode: any = {};

        lines.push(indent + 'enum ' + definition.name + ' {');
        for (let j = 0; j < definition.fields.length; j++) {
          let field = definition.fields[j];
          encode[field.name] = field.value;
          decode[field.value] = field.name;
          lines.push(indent + '  ' + field.name);
        }
        lines.push(indent + '}');
        lines.push('');

        lines.push(indent + 'namespace ' + definition.name + ' {');
        lines.push(indent + '  const _encode = ' + JSON.stringify(encode, null, 2).replace(/"/g, '').replace(/\n/g, '\n  ' + indent));
        lines.push('');

        lines.push(indent + '  const _decode = ' + JSON.stringify(decode, null, 2).replace(/"/g, '').replace(/\n/g, '\n  ' + indent));
        lines.push('');

        lines.push(indent + '  def encode(value ' + definition.name + ') int {');
        lines.push(indent + '    return _encode[value]');
        lines.push(indent + '  }');
        lines.push('');

        lines.push(indent + '  def decode(value int) ' + definition.name + ' {');
        lines.push(indent + '    if !(value in _decode) {');
        lines.push(indent + '      Kiwi.DecodeError.throwInvalidEnumValue(' + quote(definition.name) + ')');
        lines.push(indent + '    }');
        lines.push(indent + '    return _decode[value]');
        lines.push(indent + '  }');
        lines.push(indent + '}');
        lines.push('');
        break;
      }

      case 'STRUCT':
      case 'MESSAGE': {
        lines.push(indent + 'class ' + definition.name + ' {');

        for (let j = 0; j < definition.fields.length; j += 32) {
          lines.push(indent + '  var _flags' + (j >> 5) + ' = 0');
        }

        for (let j = 0; j < definition.fields.length; j++) {
          let field = definition.fields[j];
          if (field.isDeprecated) {
            continue;
          }
          lines.push(indent + '  var _' + field.name + ' ' + skewTypeForField(field) + ' = ' + skewDefaultValueForField(definitions, field));
        }

        lines.push('');

        for (let j = 0; j < definition.fields.length; j++) {
          let field = definition.fields[j];

          if (field.isDeprecated) {
            continue;
          }

          let type = skewTypeForField(field);
          let flags = '_flags' + (j >> 5);
          let mask = '' + (1 << (j % 32) >>> 0);

          lines.push(indent + '  def has_' + field.name + ' bool {');
          lines.push(indent + '    return (' + flags + ' & ' + mask + ') != 0');
          lines.push(indent + '  }');
          lines.push('');

          lines.push(indent + '  def ' + field.name + ' ' + type + ' {');
          lines.push(indent + '    assert(has_' + field.name + ')');
          lines.push(indent + '    return _' + field.name);
          lines.push(indent + '  }');
          lines.push('');

          lines.push(indent + '  def ' + field.name + '=(value ' + type + ') {');
          lines.push(indent + '    _' + field.name + ' = value');
          lines.push(indent + '    ' + flags + ' |= ' + mask);
          lines.push(indent + '  }');
          lines.push('');
        }

        lines.push(indent + '  def encode(bb Kiwi.ByteBuffer) {');

        for (let j = 0; j < definition.fields.length; j++) {
          let field = definition.fields[j];

          if (field.isDeprecated) {
            continue;
          }

          let value = '_' + field.name;
          let code;

          if (field.isArray) {
            value = 'value';
          }

          switch (field.type) {
            case 'bool': {
              code = 'bb.writeByte(' + value + ' as int)';
              break;
            }

            case 'byte': {
              code = 'bb.writeByte(' + value + ')';
              break;
            }

            case 'int': {
              code = 'bb.writeVarInt(' + value + ')';
              break;
            }

            case 'uint': {
              code = 'bb.writeVarUint(' + value + ')';
              break;
            }

            case 'float': {
              code = 'bb.writeVarFloat(' + value + ')';
              break;
            }

            case 'string': {
              code = 'bb.writeString(' + value + ')';
              break;
            }

            case 'int64': {
              code = 'bb.writeVarInt64(' + value + ')';
              break;
            }

            case 'uint64': {
              code = 'bb.writeVarUint64(' + value + ')';
              break;
            }

            default: {
              let type = definitions[field.type!];
              if (!type) {
                error('Invalid type ' + quote(field.type!) + ' for field ' + quote(field.name), field.line, field.column);
              } else if (type.kind === 'ENUM') {
                code = 'bb.writeVarUint(' + type.name + '.encode(' + value + '))';
              } else {
                code = value + '.encode(bb)';
              }
            }
          }

          let nestedIndent = indent + '    ';

          if (definition.kind === 'STRUCT') {
            lines.push(nestedIndent + 'assert(has_' + field.name + ')');
          } else {
            lines.push(nestedIndent + 'if has_' + field.name + ' {');
            nestedIndent += '  ';
          }

          if (definition.kind === 'MESSAGE') {
            lines.push(nestedIndent + 'bb.writeVarUint(' + field.value + ')');
          }

          if (field.isArray) {
            lines.push(nestedIndent + 'bb.writeVarUint(_' + field.name + '.count)');
            lines.push(nestedIndent + 'for value in _' + field.name + ' {');
            lines.push(nestedIndent + '  ' + code);
            lines.push(nestedIndent + '}');
          } else {
            lines.push(nestedIndent + code);
          }

          if (definition.kind !== 'STRUCT') {
            lines.push(indent + '    }');
          }

          lines.push('');
        }

        if (definition.kind === 'MESSAGE') {
          lines.push(indent + '    bb.writeVarUint(0)');
        } else {
          popTrailingNewline(lines);
        }
        lines.push(indent + '  }');
        lines.push('');

        lines.push(indent + '  def encode Uint8Array {');
        lines.push(indent + '    var bb = Kiwi.ByteBuffer.new');
        lines.push(indent + '    encode(bb)');
        lines.push(indent + '    return bb.toUint8Array');
        lines.push(indent + '  }');

        lines.push(indent + '}');
        lines.push('');

        lines.push(indent + 'namespace ' + definition.name + ' {');
        lines.push(indent + '  def decode(bytes Uint8Array) ' + definition.name + ' {');
        lines.push(indent + '    return decode(Kiwi.ByteBuffer.new(bytes), null)');
        lines.push(indent + '  }');
        lines.push('');
        lines.push(indent + '  def decode(bytes Uint8Array, schema BinarySchema) ' + definition.name + ' {');
        lines.push(indent + '    return decode(Kiwi.ByteBuffer.new(bytes), schema)');
        lines.push(indent + '  }');
        lines.push('');

        lines.push(indent + '  def decode(bb Kiwi.ByteBuffer, schema BinarySchema) ' + definition.name + ' {');
        lines.push(indent + '    var self = new');

        for (let j = 0; j < definition.fields.length; j++) {
          if (definition.fields[j].isArray) {
            lines.push(indent + '    var count = 0');
            break;
          }
        }

        let nestedIndent = indent + '  ';

        if (definition.kind === 'MESSAGE') {
          lines.push(indent + '    while true {');
          lines.push(indent + '      var type = bb.readVarUint');
          lines.push(indent + '      switch type {');
          lines.push(indent + '        case 0 {');
          lines.push(indent + '          break');
          lines.push(indent + '        }');
          lines.push('');
          nestedIndent += '      ';
        }

        for (let j = 0; j < definition.fields.length; j++) {
          let field = definition.fields[j];
          let code;

          switch (field.type) {
            case 'bool': {
              code = field.isDeprecated ? 'bb.readByte' : 'bb.readByte as bool';
              break;
            }

            case 'byte': {
              code = 'bb.readByte';
              break;
            }

            case 'int': {
              code = 'bb.readVarInt';
              break;
            }

            case 'uint': {
              code = 'bb.readVarUint';
              break;
            }

            case 'float': {
              code = 'bb.readVarFloat';
              break;
            }

            case 'string': {
              code = 'bb.readString';
              break;
            }

            case 'int64': {
              code = 'bb.readVarInt64';
              break;
            }

            case 'uint64': {
              code = 'bb.readVarUint64';
              break;
            }

            default: {
              let type = definitions[field.type!];
              if (!type) {
                error('Invalid type ' + quote(field.type!) + ' for field ' + quote(field.name), field.line, field.column);
              } else if (type.kind === 'ENUM') {
                code = type.name + '.decode(bb.readVarUint)';
              } else {
                code = type.name + '.decode(bb, schema)';
              }
            }
          }

          if (definition.kind === 'MESSAGE') {
            lines.push(nestedIndent + 'case ' + field.value + ' {');
          }

          if (field.isArray) {
            if (field.isDeprecated) {
              lines.push(nestedIndent + '  for i in 0..bb.readVarUint {');
              lines.push(nestedIndent + '    ' + code);
              lines.push(nestedIndent + '  }');
            } else {
              lines.push(nestedIndent + '  count = bb.readVarUint');
              lines.push(nestedIndent + '  self.' + field.name + ' = []');
              lines.push(nestedIndent + '  for array = self._' + field.name + '; count != 0; count-- {');
              lines.push(nestedIndent + '    array.append(' + code + ')');
              lines.push(nestedIndent + '  }');
            }
          } else {
            if (field.isDeprecated) {
              lines.push(nestedIndent + '  ' + code);
            } else {
              lines.push(nestedIndent + '  self.' + field.name + ' = ' + code);
            }
          }

          if (definition.kind === 'MESSAGE') {
            lines.push(nestedIndent + '}');
            lines.push('');
          }
        }

        if (definition.kind === 'MESSAGE') {
          lines.push(indent + '        default {');
          lines.push(indent + '          if schema == null { Kiwi.DecodeError.throwInvalidMessage }');
          lines.push(indent + '          else { schema.skip' + definition.name + 'Field(bb, type) }');
          lines.push(indent + '        }');
          lines.push(indent + '      }');
          lines.push(indent + '    }');
        }

        lines.push(indent + '    return self');
        lines.push(indent + '  }');
        lines.push(indent + '}');
        lines.push('');
        break;
      }

      default: {
        error('Invalid definition kind ' + quote(definition.kind), definition.line, definition.column);
        break;
      }
    }
  }

  if (schema.package !== null) {
    popTrailingNewline(lines);
    lines.push('}');
  }

  lines.push('');
  return lines.join('\n');
}
