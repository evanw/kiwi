import { Schema, Definition, Field, DefinitionKind } from "./schema";
import { error, quote } from "./util";

export let nativeTypes = [
  'bool',
  'byte',
  'float',
  'int',
  'int64',
  'string',
  'uint',
  'uint64',
];

// These are special names on the object returned by compileSchema()
export let reservedNames = [
  'ByteBuffer',
  'package',
];

let regex = /((?:-|\b)\d+\b|[=;{}]|\[\]|\[deprecated\]|\b[A-Za-z_][A-Za-z0-9_]*\b|\/\/.*|\s+)/g;
let identifier = /^[A-Za-z_][A-Za-z0-9_]*$/;
let whitespace = /^\/\/.*|\s+$/;
let integer = /^-?\d+$/;

interface Token {
  text: string
  line: number
  column: number
}

function tokenize(text: string): Token[] {
  let tokens = [];
  let column = 0;
  let line = 0;

  for (const part of text.split(regex).filter((e, i) => i & 1)) {
    if (!whitespace.test(part)) {
      tokens.push({
        text: part,
        line: line + 1,
        column: column + 1,
      });
    }

    // Keep track of the line and column counts
    let lines = part.split('\n');
    if (lines.length > 1) {
      column = 0;
      line += lines.length - 1;
    } else {
      column += lines[lines.length - 1].length;
    }
  }

  return tokens;
}

function parse(tokens: Token[]): Schema {
  function eatStr(test: string, token: Token): boolean {
    if (test === token.text) {
      index++;
      return true;
    }
    return false;
  }

  function expect(test: RegExp, expected: string, token: Token): void {
    if (!test.test(token.text)) {
      error('Expected ' + expected + ' but found ' + quote(token.text), token.line, token.column);
    }
    index++;
  }

  function expectStr(test: string, expected: string, token: Token): void {
    if (test !== token.text) {
      error('Expected ' + expected + ' but found ' + quote(token.text), token.line, token.column);
    }
    index++;
  }

  function unexpectedToken(): never {
    let token = tokens[index];
    error('Unexpected token ' + quote(token.text), token.line, token.column);
  }

  let definitions: Definition[] = [];
  let packageText = null;
  let index = 0;

  if (eatStr('package', tokens[index])) {
    const token = tokens[index];
    expect(identifier, 'identifier', token);
    expectStr(';', '";"', tokens[index]);
    packageText = token.text;
  }

  while (index < tokens.length) {
    let fields: Field[] = [];
    let kind: DefinitionKind;

    const keyw = tokens[index].text;
    if (keyw === 'enum') kind = 'ENUM';
    else if (keyw === 'struct') kind = 'STRUCT';
    else if (keyw === 'message') kind = 'MESSAGE';
    else unexpectedToken();
    index++;

    // All definitions start off the same
    let name = tokens[index];
    expect(identifier, 'identifier', name);
    expectStr('{', '"{"', tokens[index]);

    // Parse fields
    while (!eatStr('}', tokens[index])) {
      let type: string | null = null;
      let isArray = false;
      let isDeprecated = false;

      // Enums don't have types
      if (kind !== 'ENUM') {
        const token = tokens[index];
        expect(identifier, 'identifier', token);
        isArray = eatStr('[]', tokens[index]);
        type = token.text;
      }

      let field = tokens[index];
      expect(identifier, 'identifier', field);

      // Structs don't have explicit values
      let value: Token | null = null;
      if (kind !== 'STRUCT') {
        expectStr('=', '"="', tokens[index]);
        value = tokens[index];
        expect(integer, 'integer', value);

        if ((+value.text | 0) + '' !== value.text) {
          error('Invalid integer ' + quote(value.text), value.line, value.column);
        }
      }

      let deprecated = tokens[index];
      if (eatStr('[deprecated]', deprecated)) {
        if (kind !== 'MESSAGE') {
          error('Cannot deprecate this field', deprecated.line, deprecated.column);
        }

        isDeprecated = true;
      }

      expectStr(';', '";"', tokens[index]);

      fields.push({
        name: field.text,
        line: field.line,
        column: field.column,
        type: type,
        isArray: isArray,
        isDeprecated: isDeprecated,
        value: value !== null ? +value.text | 0 : fields.length + 1,
      });
    }

    definitions.push({
      name: name.text,
      line: name.line,
      column: name.column,
      kind: kind,
      fields: fields,
    });
  }

  return {
    package: packageText,
    definitions: definitions,
  };
}

function verify(root: Schema): void {
  let definedTypes = nativeTypes.slice();
  let definitions: { [name: string]: Definition } = {};

  // Define definitions
  for (let i = 0; i < root.definitions.length; i++) {
    let definition = root.definitions[i];
    if (definedTypes.indexOf(definition.name) !== -1) {
      error('The type ' + quote(definition.name) + ' is defined twice', definition.line, definition.column);
    }
    if (reservedNames.indexOf(definition.name) !== -1) {
      error('The type name ' + quote(definition.name) + ' is reserved', definition.line, definition.column);
    }
    definedTypes.push(definition.name);
    definitions[definition.name] = definition;
  }

  // Check fields
  for (let i = 0; i < root.definitions.length; i++) {
    let definition = root.definitions[i];
    let fields = definition.fields;

    if (definition.kind === 'ENUM' || fields.length === 0) {
      continue;
    }

    // Check types
    for (let j = 0; j < fields.length; j++) {
      let field = fields[j];
      if (definedTypes.indexOf(field.type!) === -1) {
        error('The type ' + quote(field.type!) + ' is not defined for field ' + quote(field.name), field.line, field.column);
      }
    }

    // Check values
    let values: number[] = [];
    for (let j = 0; j < fields.length; j++) {
      let field = fields[j];
      if (values.indexOf(field.value) !== -1) {
        error('The id for field ' + quote(field.name) + ' is used twice', field.line, field.column);
      }
      if (field.value <= 0) {
        error('The id for field ' + quote(field.name) + ' must be positive', field.line, field.column);
      }
      if (field.value > fields.length) {
        error('The id for field ' + quote(field.name) + ' cannot be larger than ' + fields.length, field.line, field.column);
      }
      values.push(field.value);
    }
  }

  // Check that structs don't contain themselves
  let state: { [name: string]: number } = {};
  let check = (name: string): boolean => {
    let definition = definitions[name];
    if (definition && definition.kind === 'STRUCT') {
      if (state[name] === 1) {
        error('Recursive nesting of ' + quote(name) + ' is not allowed', definition.line, definition.column);
      }
      if (state[name] !== 2 && definition) {
        state[name] = 1;
        let fields = definition.fields;
        for (let i = 0; i < fields.length; i++) {
          let field = fields[i];
          if (!field.isArray) {
            check(field.type!);
          }
        }
        state[name] = 2;
      }
    }
    return true;
  };
  for (let i = 0; i < root.definitions.length; i++) {
    check(root.definitions[i].name);
  }
}

export function parseSchema(text: string): Schema {
  let schema = parse(tokenize(text));
  verify(schema);
  return schema;
}
