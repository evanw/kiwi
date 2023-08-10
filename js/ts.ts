import { Schema } from "./schema";
import { error, quote } from "./util";

export function compileSchemaTypeScript(schema: Schema): string {
  var indent = '';
  var lines = [];

  if (schema.package !== null) {
    lines.push('export namespace ' + schema.package + ' {');
    indent += '  ';
  }

  for (var i = 0; i < schema.definitions.length; i++) {
    var definition = schema.definitions[i];

    if (definition.kind === 'ENUM') {
      lines.push(indent + 'export type ' + definition.name + ' =');

      for (var j = 0; j < definition.fields.length; j++) {
        lines.push(indent + '  ' + JSON.stringify(definition.fields[j].name) + (j + 1 < definition.fields.length ? ' |' : ';'));
      }

      if (!definition.fields.length) {
        lines.push(indent + '  any;');
      }

      lines.push('');
    }
  }

  for (var i = 0; i < schema.definitions.length; i++) {
    var definition = schema.definitions[i];

    if (definition.kind === 'STRUCT' || definition.kind === 'MESSAGE') {
      lines.push(indent + 'export interface ' + definition.name + ' {');

      for (var j = 0; j < definition.fields.length; j++) {
        var field = definition.fields[j];
        var type;

        if (field.isDeprecated) {
          continue;
        }

        switch (field.type) {
          case 'bool': type = 'boolean'; break;
          case 'byte': case 'int': case 'uint': case 'float': type = 'number'; break;
          case 'int64': case 'uint64': type = 'bigint'; break;
          default: type = field.type; break;
        }

        if (field.type === 'byte' && field.isArray) type = 'Uint8Array';
        else if (field.isArray) type += '[]';

        lines.push(indent + '  ' + field.name + (definition.kind === 'MESSAGE' ? '?' : '') + ': ' + type + ';');
      }

      lines.push(indent + '}');
      lines.push('');
    }

    else if (definition.kind !== 'ENUM') {
      error('Invalid definition kind ' + quote(definition.kind), definition.line, definition.column);
    }
  }

  lines.push(indent + 'export interface Schema {');

  for (var i = 0; i < schema.definitions.length; i++) {
    var definition = schema.definitions[i];

    if (definition.kind === 'ENUM') {
      lines.push(indent + '  ' + definition.name + ': any;');
    }

    else if (definition.kind === 'STRUCT' || definition.kind === 'MESSAGE') {
      lines.push(indent + '  encode' + definition.name + '(message: ' + definition.name + '): Uint8Array;');
      lines.push(indent + '  decode' + definition.name + '(buffer: Uint8Array): ' + definition.name + ';');
    }
  }

  lines.push(indent + '}');

  if (schema.package !== null) {
    lines.push('}');
  }

  lines.push('');
  return lines.join('\n');
}
