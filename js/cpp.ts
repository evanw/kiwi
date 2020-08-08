import { Schema, Definition, Field } from "./schema";
import { error, quote } from "./util";

function cppType(definitions: {[name: string]: Definition}, field: Field, isArray: boolean): string {
  let type;

  switch (field.type) {
    case 'bool': type = 'bool'; break;
    case 'byte': type = 'uint8_t'; break;
    case 'int': type = 'int32_t'; break;
    case 'uint': type = 'uint32_t'; break;
    case 'float': type = 'float'; break;
    case 'string': type = 'kiwi::String'; break;

    default: {
      let definition = definitions[field.type!];

      if (!definition) {
        error('Invalid type ' + quote(field.type!) + ' for field ' + quote(field.name), field.line, field.column);
      }

      type = definition.name;
      break;
    }
  }

  if (isArray) {
    type = 'kiwi::Array<' + type + '>';
  }

  return type;
}

function cppFieldName(field: Field): string {
  return '_data_' + field.name;
}

function cppFlagIndex(i: number): number {
  return i >> 5;
}

function cppFlagMask(i: number): number {
  return 1 << (i % 32) >>> 0;
}

function cppIsFieldPointer(definitions: {[name: string]: Definition}, field: Field): boolean {
  return !field.isArray && field.type! in definitions && definitions[field.type!].kind !== 'ENUM';
}

export function compileSchemaCPP(schema: Schema): string {
  let definitions: {[name: string]: Definition} = {};
  let cpp: string[] = [];

  cpp.push('#include "kiwi.h"');
  cpp.push('');

  if (schema.package !== null) {
    cpp.push('namespace ' + schema.package + ' {');
    cpp.push('');
    cpp.push('#ifndef INCLUDE_' + schema.package.toUpperCase() + '_H');
    cpp.push('#define INCLUDE_' + schema.package.toUpperCase() + '_H');
    cpp.push('');
  }

  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    definitions[definition.name] = definition;
  }

  cpp.push('class BinarySchema {');
  cpp.push('public:');
  cpp.push('  bool parse(kiwi::ByteBuffer &bb);');
  cpp.push('  const kiwi::BinarySchema &underlyingSchema() const { return _schema; }');

  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    if (definition.kind === 'MESSAGE') {
      cpp.push('  bool skip' + definition.name + 'Field(kiwi::ByteBuffer &bb, uint32_t id) const;');
    }
  }

  cpp.push('');
  cpp.push('private:');
  cpp.push('  kiwi::BinarySchema _schema;');

  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    if (definition.kind === 'MESSAGE') {
      cpp.push('  uint32_t _index' + definition.name + ' = 0;');
    }
  }

  cpp.push('};');
  cpp.push('');

  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];

    if (definition.kind === 'ENUM') {
      cpp.push('enum class ' + definition.name + ' : uint32_t {');
      for (let j = 0; j < definition.fields.length; j++) {
        let field = definition.fields[j];
        cpp.push('  ' + field.name + ' = ' + field.value + ',');
      }
      cpp.push('};');
      cpp.push('');
    }

    else if (definition.kind !== 'STRUCT' && definition.kind !== 'MESSAGE') {
      error('Invalid definition kind ' + quote(definition.kind), definition.line, definition.column);
    }
  }

  for (let pass = 0; pass < 3; pass++) {
    let newline = false;

    if (pass === 2) {
      if (schema.package !== null) {
        cpp.push('#endif');
      }

      cpp.push('#ifdef IMPLEMENT_SCHEMA_H');
      cpp.push('');

      cpp.push('bool BinarySchema::parse(kiwi::ByteBuffer &bb) {');
      cpp.push('  if (!_schema.parse(bb)) return false;');

      for (let i = 0; i < schema.definitions.length; i++) {
        let definition = schema.definitions[i];
        if (definition.kind === 'MESSAGE') {
          cpp.push('  _schema.findDefinition("' + definition.name + '", _index' + definition.name + ');');
        }
      }

      cpp.push('  return true;');
      cpp.push('}');
      cpp.push('');

      for (let i = 0; i < schema.definitions.length; i++) {
        let definition = schema.definitions[i];
        if (definition.kind === 'MESSAGE') {
          cpp.push('bool BinarySchema::skip' + definition.name + 'Field(kiwi::ByteBuffer &bb, uint32_t id) const {');
          cpp.push('  return _schema.skipField(bb, _index' + definition.name + ', id);');
          cpp.push('}');
          cpp.push('');
        }
      }
    }

    for (let i = 0; i < schema.definitions.length; i++) {
      let definition = schema.definitions[i];

      if (definition.kind === 'ENUM') {
        continue;
      }

      let fields = definition.fields;

      if (pass === 0) {
        cpp.push('class ' + definition.name + ';');
        newline = true;
      }

      else if (pass === 1) {
        cpp.push('class ' + definition.name + ' {');
        cpp.push('public:');

        // This may not actually be used, so silence warnings about "Private fields '_flags' is not used"
        cpp.push('  ' + definition.name + '() { (void)_flags; }');
        cpp.push('');

        for (let j = 0; j < fields.length; j++) {
          let field = fields[j];

          if (field.isDeprecated) {
            continue;
          }

          let name = cppFieldName(field);
          let type = cppType(definitions, field, field.isArray);
          let flagIndex = cppFlagIndex(j);
          let flagMask = cppFlagMask(j);

          if (cppIsFieldPointer(definitions, field)) {
            cpp.push('  ' + type + ' *' + field.name + '();');
            cpp.push('  const ' + type + ' *' + field.name + '() const;');
            cpp.push('  void set_' + field.name + '(' + type + ' *value);');
          }

          else if (field.isArray) {
            cpp.push('  ' + type + ' *' + field.name + '();');
            cpp.push('  const ' + type + ' *' + field.name + '() const;');
            cpp.push('  ' + type + ' &set_' + field.name + '(kiwi::MemoryPool &pool, uint32_t count);');
          }

          else {
            cpp.push('  ' + type + ' *' + field.name + '();');
            cpp.push('  const ' + type + ' *' + field.name + '() const;');
            cpp.push('  void set_' + field.name + '(const ' + type + ' &value);');
          }

          cpp.push('');
        }

        cpp.push('  bool encode(kiwi::ByteBuffer &bb);');
        cpp.push('  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);');
        cpp.push('');
        cpp.push('private:');
        cpp.push('  uint32_t _flags[' + (fields.length + 31 >> 5) + '] = {};');

        // Sort fields by size since that makes the resulting struct smaller
        let sizes: {[type: string]: number} = {'bool': 1, 'byte': 1, 'int': 4, 'uint': 4, 'float': 4};
        let sortedFields = fields.slice().sort(function(a, b) {
          let sizeA = !a.isArray && sizes[a.type!] || 8;
          let sizeB = !b.isArray && sizes[b.type!] || 8;
          if (sizeA !== sizeB) return sizeB - sizeA;
          return fields.indexOf(a) - fields.indexOf(b); // Make sure the sort is stable
        });

        for (let j = 0; j < sortedFields.length; j++) {
          let field = sortedFields[j];

          if (field.isDeprecated) {
            continue;
          }

          let name = cppFieldName(field);
          let type = cppType(definitions, field, field.isArray);

          if (cppIsFieldPointer(definitions, field)) {
            cpp.push('  ' + type + ' *' + name + ' = {};');
          } else {
            cpp.push('  ' + type + ' ' + name + ' = {};');
          }
        }

        cpp.push('};');
        cpp.push('');
      }

      else {
        for (let j = 0; j < fields.length; j++) {
          let field = fields[j];
          let name = cppFieldName(field);
          let type = cppType(definitions, field, field.isArray);
          let flagIndex = cppFlagIndex(j);
          let flagMask = cppFlagMask(j);

          if (field.isDeprecated) {
            continue;
          }

          if (cppIsFieldPointer(definitions, field)) {
            cpp.push(type + ' *' + definition.name + '::' + field.name + '() {');
            cpp.push('  return ' + name + ';');
            cpp.push('}');
            cpp.push('');

            cpp.push('const ' + type + ' *' + definition.name + '::' + field.name + '() const {');
            cpp.push('  return ' + name + ';');
            cpp.push('}');
            cpp.push('');

            cpp.push('void ' + definition.name + '::set_' + field.name + '(' + type + ' *value) {');
            cpp.push('  ' + name + ' = value;');
            cpp.push('}');
            cpp.push('');
          }

          else if (field.isArray) {
            cpp.push(type + ' *' + definition.name + '::' + field.name + '() {');
            cpp.push('  return _flags[' + flagIndex + '] & ' + flagMask + ' ? &' + name + ' : nullptr;');
            cpp.push('}');
            cpp.push('');

            cpp.push('const ' + type + ' *' + definition.name + '::' + field.name + '() const {');
            cpp.push('  return _flags[' + flagIndex + '] & ' + flagMask + ' ? &' + name + ' : nullptr;');
            cpp.push('}');
            cpp.push('');

            cpp.push(type + ' &' + definition.name + '::set_' + field.name + '(kiwi::MemoryPool &pool, uint32_t count) {');
            cpp.push('  _flags[' + flagIndex + '] |= ' + flagMask + '; return ' + name + ' = pool.array<' + cppType(definitions, field, false) + '>(count);');
            cpp.push('}');
            cpp.push('');
          }

          else {
            cpp.push(type + ' *' + definition.name + '::' + field.name + '() {');
            cpp.push('  return _flags[' + flagIndex + '] & ' + flagMask + ' ? &' + name + ' : nullptr;');
            cpp.push('}');
            cpp.push('');

            cpp.push('const ' + type + ' *' + definition.name + '::' + field.name + '() const {');
            cpp.push('  return _flags[' + flagIndex + '] & ' + flagMask + ' ? &' + name + ' : nullptr;');
            cpp.push('}');
            cpp.push('');

            cpp.push('void ' + definition.name + '::set_' + field.name + '(const ' + type + ' &value) {');
            cpp.push('  _flags[' + flagIndex + '] |= ' + flagMask + '; ' + name + ' = value;');
            cpp.push('}');
            cpp.push('');
          }
        }

        cpp.push('bool ' + definition.name + '::encode(kiwi::ByteBuffer &_bb) {');

        for (let j = 0; j < fields.length; j++) {
          let field = fields[j];

          if (field.isDeprecated) {
            continue;
          }

          let name = cppFieldName(field);
          let value = field.isArray ? '_it' : name;
          let flagIndex = cppFlagIndex(j);
          let flagMask = cppFlagMask(j);
          let code;

          switch (field.type) {
            case 'bool': {
              code = '_bb.writeByte(' + value + ');';
              break;
            }

            case 'byte': {
              code = '_bb.writeByte(' + value + ');';
              break;
            }

            case 'int': {
              code = '_bb.writeVarInt(' + value + ');';
              break;
            }

            case 'uint': {
              code = '_bb.writeVarUint(' + value + ');';
              break;
            }

            case 'float': {
              code = '_bb.writeVarFloat(' + value + ');';
              break;
            }

            case 'string': {
              code = '_bb.writeString(' + value + '.c_str());';
              break;
            }

            default: {
              let type = definitions[field.type!];

              if (!type) {
                error('Invalid type ' + quote(field.type!) + ' for field ' + quote(field.name), field.line, field.column);
              }

              else if (type.kind === 'ENUM') {
                code = '_bb.writeVarUint(static_cast<uint32_t>(' + value + '));';
              }

              else {
                code = 'if (!' + value + (cppIsFieldPointer(definitions, field) ? '->' : '.') + 'encode(_bb)) return false;';
              }
            }
          }

          let indent = '  ';
          if (definition.kind === 'STRUCT') {
            cpp.push('  if (' + field.name + '() == nullptr) return false;');
          } else {
            cpp.push('  if (' + field.name + '() != nullptr) {');
            indent = '    ';
          }

          if (definition.kind === 'MESSAGE') {
            cpp.push(indent + '_bb.writeVarUint(' + field.value + ');');
          }

          if (field.isArray) {
            cpp.push(indent + '_bb.writeVarUint(' + name + '.size());');
            cpp.push(indent + 'for (' + cppType(definitions, field, false) + ' &_it : ' + name + ') ' + code);
          } else {
            cpp.push(indent + code);
          }

          if (definition.kind !== 'STRUCT') {
            cpp.push('  }');
          }
        }

        if (definition.kind === 'MESSAGE') {
          cpp.push('  _bb.writeVarUint(0);');
        }

        cpp.push('  return true;');
        cpp.push('}');
        cpp.push('');

        cpp.push('bool ' + definition.name + '::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {');

        for (let j = 0; j < fields.length; j++) {
          if (fields[j].isArray) {
            cpp.push('  uint32_t _count;');
            break;
          }
        }

        if (definition.kind === 'MESSAGE') {
          cpp.push('  while (true) {');
          cpp.push('    uint32_t _type;');
          cpp.push('    if (!_bb.readVarUint(_type)) return false;');
          cpp.push('    switch (_type) {');
          cpp.push('      case 0:');
          cpp.push('        return true;');
        }

        for (let j = 0; j < fields.length; j++) {
          let field = fields[j];
          let name = cppFieldName(field);
          let value = field.isArray ? '_it' : name;
          let isPointer = cppIsFieldPointer(definitions, field);
          let code;

          switch (field.type) {
            case 'bool': {
              code = '_bb.readByte(' + value + ')';
              break;
            }

            case 'byte': {
              code = '_bb.readByte(' + value + ')';
              break;
            }

            case 'int': {
              code = '_bb.readVarInt(' + value + ')';
              break;
            }

            case 'uint': {
              code = '_bb.readVarUint(' + value + ')';
              break;
            }

            case 'float': {
              code = '_bb.readVarFloat(' + value + ')';
              break;
            }

            case 'string': {
              code = '_bb.readString(' + value + ', _pool)';
              break;
            }

            default: {
              let type = definitions[field.type!];

              if (!type) {
                error('Invalid type ' + quote(field.type!) + ' for field ' + quote(field.name), field.line, field.column);
              }

              else if (type.kind === 'ENUM') {
                code = '_bb.readVarUint(reinterpret_cast<uint32_t &>(' + value + '))';
              }

              else {
                code = value + (isPointer ? '->' : '.') + 'decode(_bb, _pool, _schema)';
              }
            }
          }

          let type = cppType(definitions, field, false);
          let indent = '  ';

          if (definition.kind === 'MESSAGE') {
            cpp.push('      case ' + field.value + ': {');
            indent = '        ';
          }

          if (field.isArray) {
            cpp.push(indent + 'if (!_bb.readVarUint(_count)) return false;');
            if (field.isDeprecated) {
              cpp.push(indent + 'for (' + type + ' &_it : _pool.array<' + cppType(definitions, field, false) + '>(_count)) if (!' + code + ') return false;');
            } else {
              cpp.push(indent + 'for (' + type + ' &_it : set_' + field.name + '(_pool, _count)) if (!' + code + ') return false;');
            }
          }

          else {
            if (field.isDeprecated) {
              if (isPointer) {
                cpp.push(indent + type + ' *' + name + ' = _pool.allocate<' + type + '>();');
              } else {
                cpp.push(indent + type + ' ' + name + ' = {};');
              }

              cpp.push(indent + 'if (!' + code + ') return false;');
            }

            else {
              if (isPointer) {
                cpp.push(indent + name + ' = _pool.allocate<' + type + '>();');
              }

              cpp.push(indent + 'if (!' + code + ') return false;');

              if (!isPointer) {
                cpp.push(indent + 'set_' + field.name + '(' + name + ');');
              }
            }
          }

          if (definition.kind === 'MESSAGE') {
            cpp.push('        break;');
            cpp.push('      }');
          }
        }

        if (definition.kind === 'MESSAGE') {
          cpp.push('      default: {');
          cpp.push('        if (!_schema || !_schema->skip' + definition.name + 'Field(_bb, _type)) return false;');
          cpp.push('        break;');
          cpp.push('      }');
          cpp.push('    }');
          cpp.push('  }');
        }

        else {
          cpp.push('  return true;');
        }

        cpp.push('}');
        cpp.push('');
      }
    }

    if (pass === 2) {
      cpp.push('#endif');
      cpp.push('');
    }

    else if (newline) cpp.push('');
  }

  if (schema.package !== null) {
    cpp.push('}');
    cpp.push('');
  }

  return cpp.join('\n');
}
