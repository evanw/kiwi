import { Schema } from "./schema";
import { error, quote } from "./util";

export function compileSchemaSkewTypes(schema: Schema): string {
  var indent = '';
  var lines = [];

  if (schema.package !== null) {
    lines.push('namespace ' + schema.package + ' {');
    indent += '  ';
  }

  for (var i = 0; i < schema.definitions.length; i++) {
    var definition = schema.definitions[i];

    if (definition.kind === 'ENUM') {
      lines.push(indent + 'type ' + definition.name + ' : string {');
      lines.push(indent + '  @alwaysinline');
      lines.push(indent + '  def toString string { return self as string }');
      lines.push(indent + '}');

      if (definition.fields.length > 0) {
        lines.push(indent + 'namespace ' + definition.name + ' {');

        for (var j = 0; j < definition.fields.length; j++) {
          lines.push(indent + '  @alwaysinline')
          lines.push(indent + '  def ' + definition.fields[j].name + ' ' + definition.name +
            ' { return ' + JSON.stringify(definition.fields[j].name) + ' as ' + definition.name + ' }');
        }

        lines.push(indent + '}');
      }

      lines.push('');
    }
  }

  for (var i = 0; i < schema.definitions.length; i++) {
    var definition = schema.definitions[i];

    if (definition.kind === 'STRUCT' || definition.kind === 'MESSAGE') {
      lines.push(indent + '@import');
      lines.push(indent + 'class ' + definition.name + ' {');

      for (var j = 0; j < definition.fields.length; j++) {
        var field = definition.fields[j];
        var type;

        if (field.isDeprecated) {
          continue;
        }

        switch (field.type) {
          case 'byte': case 'uint': type = 'int'; break;
          case 'float': type = 'double'; break;
          case 'int64': case 'uint64': type = 'BigInt'; break;
          default: type = field.type; break;
        }

        if (field.type === 'byte' && field.isArray) type = 'Uint8Array';
        else if (field.isArray) type = 'List<' + type + '>';

        lines.push(indent + '  var ' + field.name + ' ' + type);

        lines.push(indent + '  @alwaysinline');
        lines.push(indent + '  def has_' + field.name + ' bool { return self.' + field.name + ' != dynamic.void(0) }');
      }

      lines.push(indent + '}');
      lines.push('');

      lines.push(indent + 'namespace ' + definition.name + ' {');
      lines.push(indent + '  @alwaysinline');
      lines.push(indent + '  def new ' + definition.name + ' { return {} as dynamic }');
      lines.push(indent + '}');
      lines.push('');
    }

    else if (definition.kind !== 'ENUM') {
      error('Invalid definition kind ' + quote(definition.kind), definition.line, definition.column);
    }
  }

  lines.push(indent + '@import');
  lines.push(indent + 'class Schema {');

  for (var i = 0; i < schema.definitions.length; i++) {
    var definition = schema.definitions[i];

    if (definition.kind === 'ENUM') {
      lines.push(indent + '  const ' + definition.name + ' dynamic');
    }

    else if (definition.kind === 'STRUCT' || definition.kind === 'MESSAGE') {
      lines.push(indent + '  def encode' + definition.name + '(message ' + definition.name + ') Uint8Array');
      lines.push(indent + '  def decode' + definition.name + '(buffer Uint8Array) ' + definition.name);
    }
  }

  lines.push(indent + '}');

  if (schema.package !== null) {
    lines.push('}');
  }

  lines.push('');
  return lines.join('\n');
}
