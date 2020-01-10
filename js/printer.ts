import { Schema } from "./schema";

export function prettyPrintSchema(schema: Schema): string {
  let definitions = schema.definitions;
  let text = '';

  if (schema.package !== null) {
    text += 'package ' + schema.package + ';\n';
  }

  for (let i = 0; i < definitions.length; i++) {
    let definition = definitions[i];
    if (i > 0 || schema.package !== null) text += '\n';
    text += definition.kind.toLowerCase() + ' ' + definition.name + ' {\n';

    for (let j = 0; j < definition.fields.length; j++) {
      let field = definition.fields[j];
      text += '  ';
      if (definition.kind !== 'ENUM') {
        text += field.type;
        if (field.isArray) {
          text += '[]';
        }
        text += ' ';
      }
      text += field.name;
      if (definition.kind !== 'STRUCT') {
        text += ' = ' + field.value;
      }
      if (field.isDeprecated) {
        text += ' [deprecated]';
      }
      text += ';\n';
    }

    text += '}\n';
  }

  return text;
}
