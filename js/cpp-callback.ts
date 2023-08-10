import { Schema, Definition, Field } from "./schema";

interface Argument {
  type: string;
  name: string;
}

function argumentForField(definitions: { [name: string]: Definition }, type: string | null, name: string): Argument | null {
  switch (type) {
    case 'bool': return { type: 'bool ', name: name };
    case 'byte': return { type: 'uint8_t ', name: name };
    case 'int': return { type: 'int32_t ', name: name };
    case 'uint': return { type: 'uint32_t ', name: name };
    case 'float': return { type: 'float ', name: name };
    case 'string': return { type: 'const char *', name: name };
    case 'int64': return { type: 'int64_t ', name: name };
    case 'uint64': return { type: 'uint64_t ', name: name };
    default: {
      let definition = definitions[type!];
      if (definition.kind === 'ENUM') return { type: definition.name + ' ', name: name };
      return null;
    }
  }
}

function extractStructArguments(definitions: { [name: string]: Definition }, prefix: string, fields: Field[], allowArrays: boolean): Argument[] | null {
  let args: Argument[] = [];

  for (let i = 0; i < fields.length; i++) {
    let field = fields[i];
    let name = prefix + field.name;

    if (field.isArray && !allowArrays) {
      return null;
    }

    let arg = argumentForField(definitions, field.type, name);
    if (arg !== null) {
      args.push(arg);
      continue;
    }

    let type = definitions[field.type!];
    if (type.kind !== 'STRUCT') {
      return null;
    }

    let typeArgs = extractStructArguments(definitions, name + '_', type.fields, false);
    if (typeArgs === null) {
      return null;
    }
    args.push.apply(args, typeArgs);
  }

  return args;
}

function argToDeclaration(arg: Argument): string {
  return arg.type + arg.name;
}

function argToName(arg: Argument): string {
  return arg.name;
}

function argToNotRead(arg: Argument): string {
  switch (arg.type) {
    case 'bool ':
    case 'uint8_t ': return '!bb.readByte(' + arg.name + ')';
    case 'int32_t ': return '!bb.readVarInt(' + arg.name + ')';
    case 'uint32_t ': return '!bb.readVarUint(' + arg.name + ')';
    case 'float ': return '!bb.readVarFloat(' + arg.name + ')';
    case 'const char *': return '!bb.readString(' + arg.name + ')';
    case 'int64_t ': return '!bb.readVarInt64(' + arg.name + ')';
    case 'uint64_t ': return '!bb.readVarUint64(' + arg.name + ')';
    default: return '!bb.readVarUint(reinterpret_cast<uint32_t &>(' + arg.name + '))';
  }
}

function argToWrite(arg: Argument): string {
  switch (arg.type) {
    case 'bool ':
    case 'uint8_t ': return '_bb.writeByte(' + arg.name + ')';
    case 'int32_t ': return '_bb.writeVarInt(' + arg.name + ')';
    case 'uint32_t ': return '_bb.writeVarUint(' + arg.name + ')';
    case 'float ': return '_bb.writeVarFloat(' + arg.name + ')';
    case 'const char *': return '_bb.writeString(' + arg.name + ')';
    case 'int64_t ': return '_bb.writeVarInt64(' + arg.name + ')';
    case 'uint64_t ': return '_bb.writeVarUint64(' + arg.name + ')';
    default: return '_bb.writeVarUint(static_cast<uint32_t>(' + arg.name + '))';
  }
}

function emitReadField(cpp: string[], definitions: { [name: string]: Definition }, definition: Definition, field: Field, indent: string): void {
  let name = field.name;

  if (field.isArray) {
    let count = '_' + name + '_count';
    cpp.push(indent + 'uint32_t ' + count + ';');
    cpp.push(indent + 'if (!bb.readVarUint(' + count + ')) return false;');
    if (!field.isDeprecated) {
      cpp.push(indent + 'visitor.visit' + definition.name + '_' + field.name + '_count(' + count + ');');
    }
    cpp.push(indent + 'while (' + count + '-- > 0) {');
    indent += '  ';
    name += '_element';
  }

  let args = extractStructArguments(definitions, '', [field], true);
  if (args !== null) {
    for (let i = 0; i < args.length; i++) {
      cpp.push(indent + argToDeclaration(args[i]) + ';');
    }
    cpp.push(indent + 'if (' + args.map(argToNotRead).join(' || ') + ') return false;');
    if (!field.isDeprecated) {
      cpp.push(indent + 'visitor.visit' + definition.name + '_' + name + '(' + args.map(argToName).join(', ') + ');');
    }
  } else {
    if (!field.isDeprecated) {
      cpp.push(indent + 'visitor.visit' + definition.name + '_' + name + '();');
    }
    let type = definitions[field.type!];
    cpp.push(indent + 'if (!parse' + type.name + '(bb, visitor)) return false;');
  }

  if (field.isArray) {
    cpp.push(indent.slice(2) + '}');
  }
}

export function compileSchemaCallbackCPP(schema: Schema): string {
  let definitions: { [name: string]: Definition } = {};
  let cpp: string[] = [];

  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    definitions[definition.name] = definition;
  }

  if (schema.package !== null) {
    cpp.push('#ifndef INCLUDE_' + schema.package.toUpperCase() + '_H');
    cpp.push('#define INCLUDE_' + schema.package.toUpperCase() + '_H');
    cpp.push('');
  }

  cpp.push('#include "kiwi.h"');
  cpp.push('');

  if (schema.package !== null) {
    cpp.push('namespace ' + schema.package + ' {');
    cpp.push('');
  }

  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    if (definition.kind === 'ENUM') {
      cpp.push('enum class ' + definition.name + ' : uint32_t {');
      for (let j = 0; j < definition.fields.length; j++) {
        let field = definition.fields[j];
        if (!field.isDeprecated) {
          cpp.push('  ' + field.name + ' = ' + field.value + ',');
        }
      }
      cpp.push('};');
      cpp.push('');
    }
  }

  for (let pass = 0; pass < 2; pass++) {
    let suffix = ') = 0;';
    if (pass === 0) {
      cpp.push('class Visitor {');
      cpp.push('public:');
    } else {
      cpp.push('class Writer : public Visitor {');
      cpp.push('private:');
      cpp.push('  kiwi::ByteBuffer &_bb;');
      cpp.push('public:');
      cpp.push('  Writer(kiwi::ByteBuffer &bb) : _bb(bb) {}');
      suffix = ') override;';
    }

    for (let i = 0; i < schema.definitions.length; i++) {
      let definition = schema.definitions[i];

      if (definition.kind === 'STRUCT') {
        let args = extractStructArguments(definitions, '', definition.fields, false);
        if (args !== null) {
          cpp.push('  virtual void visit' + definition.name + '(' + args.map(argToDeclaration).join(', ') + suffix);
          continue;
        }
      }

      if (definition.kind === 'STRUCT' || definition.kind === 'MESSAGE') {
        cpp.push('  virtual void begin' + definition.name + '(' + suffix);
        for (let j = 0; j < definition.fields.length; j++) {
          let field = definition.fields[j];
          if (field.isDeprecated) {
            continue;
          }
          let name = field.name;
          if (field.isArray) {
            cpp.push('  virtual void visit' + definition.name + '_' + field.name + '_count(uint32_t size' + suffix);
            name += '_element';
          }
          let args = extractStructArguments(definitions, '', [field], true);
          if (args !== null) {
            cpp.push('  virtual void visit' + definition.name + '_' + name + '(' + args.map(argToDeclaration).join(', ') + suffix);
          } else {
            cpp.push('  virtual void visit' + definition.name + '_' + name + '(' + suffix);
          }
        }
        cpp.push('  virtual void end' + definition.name + '(' + suffix);
      }
    }

    cpp.push('};');
    cpp.push('');
  }

  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    if (definition.kind === 'STRUCT' || definition.kind === 'MESSAGE') {
      cpp.push('bool parse' + definition.name + '(kiwi::ByteBuffer &bb, Visitor &visitor);');
    }
  }

  cpp.push('');
  cpp.push('#ifdef IMPLEMENT_SCHEMA_H');
  cpp.push('');

  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];

    if (definition.kind === 'STRUCT') {
      let args = extractStructArguments(definitions, '', definition.fields, false);
      if (args !== null) {
        cpp.push('bool parse' + definition.name + '(kiwi::ByteBuffer &bb, Visitor &visitor) {');
        for (let j = 0; j < args.length; j++) {
          cpp.push('  ' + argToDeclaration(args[j]) + ';');
        }
        cpp.push('  if (' + args.map(argToNotRead).join(' || ') + ') return false;');
        cpp.push('  visitor.visit' + definition.name + '(' + args.map(argToName).join(', ') + ');');
        cpp.push('  return true;');
        cpp.push('}');
        cpp.push('');
      } else {
        cpp.push('bool parse' + definition.name + '(kiwi::ByteBuffer &bb, Visitor &visitor) {');
        cpp.push('  visitor.begin' + definition.name + '();');
        for (let j = 0; j < definition.fields.length; j++) {
          let field = definition.fields[j];
          emitReadField(cpp, definitions, definition, field, '  ');
        }
        cpp.push('  visitor.end' + definition.name + '();');
        cpp.push('  return true;');
        cpp.push('}');
        cpp.push('');
      }
    }

    else if (definition.kind === 'MESSAGE') {
      cpp.push('bool parse' + definition.name + '(kiwi::ByteBuffer &bb, Visitor &visitor) {');
      cpp.push('  visitor.begin' + definition.name + '();');
      cpp.push('  while (true) {');
      cpp.push('    uint32_t _type;');
      cpp.push('    if (!bb.readVarUint(_type)) return false;');
      cpp.push('    switch (_type) {');
      cpp.push('      case 0: {');
      cpp.push('        visitor.end' + definition.name + '();');
      cpp.push('        return true;');
      cpp.push('      }');
      for (let j = 0; j < definition.fields.length; j++) {
        let field = definition.fields[j];
        cpp.push('      case ' + field.value + ': {');
        emitReadField(cpp, definitions, definition, field, '        ');
        cpp.push('        break;');
        cpp.push('      }');
      }
      cpp.push('      default: return false;');
      cpp.push('    }');
      cpp.push('  }');
      cpp.push('}');
      cpp.push('');
    }
  }

  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];

    if (definition.kind === 'STRUCT') {
      let args = extractStructArguments(definitions, '', definition.fields, false);
      if (args !== null) {
        cpp.push('void Writer::visit' + definition.name + '(' + args.map(argToDeclaration).join(', ') + ') {');
        for (let j = 0; j < args.length; j++) {
          cpp.push('  ' + argToWrite(args[j]) + ';');
        }
        cpp.push('}');
        cpp.push('');
        continue;
      }
    }

    if (definition.kind === 'STRUCT' || definition.kind === 'MESSAGE') {
      cpp.push('void Writer::begin' + definition.name + '() {');
      cpp.push('}');
      cpp.push('');

      for (let j = 0; j < definition.fields.length; j++) {
        let field = definition.fields[j];
        if (field.isDeprecated) {
          continue;
        }
        let name = field.name;
        if (field.isArray) {
          cpp.push('void Writer::visit' + definition.name + '_' + field.name + '_count(uint32_t size) {');
          if (definition.kind === 'MESSAGE') {
            cpp.push('  _bb.writeVarUint(' + field.value + ');');
          }
          cpp.push('  _bb.writeVarUint(size);');
          cpp.push('}');
          cpp.push('');
          name += '_element';
        }
        let args = extractStructArguments(definitions, '', [field], true);
        if (args !== null) {
          cpp.push('void Writer::visit' + definition.name + '_' + name + '(' + args.map(argToDeclaration).join(', ') + ') {');
          if (definition.kind === 'MESSAGE' && !field.isArray) {
            cpp.push('  _bb.writeVarUint(' + field.value + ');');
          }
          for (let k = 0; k < args.length; k++) {
            cpp.push('  ' + argToWrite(args[k]) + ';');
          }
          cpp.push('}');
          cpp.push('');
        } else {
          cpp.push('void Writer::visit' + definition.name + '_' + name + '() {');
          if (definition.kind === 'MESSAGE' && !field.isArray) {
            cpp.push('  _bb.writeVarUint(' + field.value + ');');
          }
          cpp.push('}');
          cpp.push('');
        }
      }

      cpp.push('void Writer::end' + definition.name + '() {');
      if (definition.kind === 'MESSAGE') {
        cpp.push('  _bb.writeVarUint(0);');
      }
      cpp.push('}');
      cpp.push('');
    }
  }

  cpp.push('#endif');
  cpp.push('');

  if (schema.package !== null) {
    cpp.push('}');
    cpp.push('');
    cpp.push('#endif');
    cpp.push('');
  }

  return cpp.join('\n');
}
