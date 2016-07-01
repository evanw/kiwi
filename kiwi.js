var kiwi = exports || kiwi || {}, exports;

(function() {

  function quote(text) {
    return JSON.stringify(text);
  }

  function error(text, line, column) {
    var error = new Error(text);
    error.line = line;
    error.column = column;
    throw error;
  }

  function convertSchema(schema) {
    if (typeof schema === 'string') {
      return kiwi.parseSchema(schema);
    }

    if (schema instanceof Uint8Array || schema instanceof kiwi.ByteBuffer) {
      return kiwi.decodeBinarySchema(schema);
    }

    return schema;
  }

// ByteBuffer
(function() {
  var int32 = new Int32Array(1);
  var float32 = new Float32Array(int32.buffer);

  function ByteBuffer(data) {
    if (data && !(data instanceof Uint8Array)) {
      throw new Error('Must initialize a ByteBuffer with a Uint8Array');
    }
    this._data = data || new Uint8Array(256);
    this._index = 0;
    this.length = data ? data.length : 0;
  }

  ByteBuffer.prototype.toUint8Array = function() {
    return this._data.subarray(0, this.length);
  };

  ByteBuffer.prototype.readByte = function() {
    if (this._index + 1 > this._data.length) {
      throw new Error('Index out of bounds');
    }
    return this._data[this._index++];
  };

  ByteBuffer.prototype.readVarFloat = function() {
    var index = this._index;
    var data = this._data;
    var length = data.length;

    // Optimization: use a single byte to store zero
    if (index + 1 > length) {
      throw new Error('Index out of bounds');
    }
    var first = data[index];
    if (first === 0) {
      this._index = index + 1;
      return 0;
    }

    // Endian-independent 32-bit read
    if (index + 4 > length) {
      throw new Error('Index out of bounds');
    }
    var bits = first | (data[index + 1] << 8) | (data[index + 2] << 16) | (data[index + 3] << 24);
    this._index = index + 4;

    // Move the exponent back into place
    bits = (bits << 23) | (bits >>> 9);

    // Reinterpret as a floating-point number
    int32[0] = bits;
    return float32[0];
  };

  ByteBuffer.prototype.readVarUint = function() {
    var value = 0;
    var shift = 0;
    do {
      var byte = this.readByte();
      value |= (byte & 127) << shift;
      shift += 7;
    } while (byte & 128 && shift < 35);
    return value >>> 0;
  };

  ByteBuffer.prototype.readVarInt = function() {
    var value = this.readVarUint() | 0;
    return value & 1 ? ~(value >>> 1) : value >>> 1;
  };

  ByteBuffer.prototype.readString = function() {
    var result = '';

    while (true) {
      var codePoint;

      // Decode UTF-8
      var a = this.readByte();
      if (a < 0xC0) {
        codePoint = a;
      } else {
        var b = this.readByte();
        if (a < 0xE0) {
          codePoint = ((a & 0x1F) << 6) | (b & 0x3F);
        } else {
          var c = this.readByte();
          if (a < 0xF0) {
            codePoint = ((a & 0x0F) << 12) | ((b & 0x3F) << 6) | (c & 0x3F);
          } else {
            var d = this.readByte();
            codePoint = ((a & 0x07) << 18) | ((b & 0x3F) << 12) | ((c & 0x3F) << 6) | (d & 0x3F);
          }
        }
      }

      // Strings are null-terminated
      if (codePoint === 0) {
        break;
      }

      // Encode UTF-16
      if (codePoint < 0x10000) {
        result += String.fromCharCode(codePoint);
      } else {
        codePoint -= 0x10000;
        result += String.fromCharCode((codePoint >> 10) + 0xD800, (codePoint & ((1 << 10) - 1)) + 0xDC00);
      }
    }

    return result;
  };

  ByteBuffer.prototype._growBy = function(amount) {
    if (this.length + amount > this._data.length) {
      var data = new Uint8Array(this.length + amount << 1);
      data.set(this._data);
      this._data = data;
    }
    this.length += amount;
  };

  ByteBuffer.prototype.writeByte = function(value) {
    var index = this.length;
    this._growBy(1);
    this._data[index] = value;
  };

  ByteBuffer.prototype.writeVarFloat = function(value) {
    var index = this.length;

    // Reinterpret as an integer
    float32[0] = value;
    var bits = int32[0];

    // Move the exponent to the first 8 bits
    bits = (bits >>> 23) | (bits << 9);

    // Optimization: use a single byte to store zero and denormals (check for an exponent of 0)
    if ((bits & 255) === 0) {
      this.writeByte(0);
      return;
    }

    // Endian-independent 32-bit write
    this._growBy(4);
    var data = this._data;
    data[index] = bits;
    data[index + 1] = bits >> 8;
    data[index + 2] = bits >> 16;
    data[index + 3] = bits >> 24;
  };

  ByteBuffer.prototype.writeVarUint = function(value) {
    do {
      var byte = value & 127;
      value >>>= 7;
      this.writeByte(value ? byte | 128 : byte);
    } while (value);
  };

  ByteBuffer.prototype.writeVarInt = function(value) {
    this.writeVarUint((value << 1) ^ (value >> 31));
  };

  ByteBuffer.prototype.writeString = function(value) {
    var codePoint;

    for (var i = 0; i < value.length; i++) {
      // Decode UTF-16
      var a = value.charCodeAt(i);
      if (i + 1 === value.length || a < 0xD800 || a >= 0xDC00) {
        codePoint = a;
      } else {
        var b = value.charCodeAt(++i);
        codePoint = (a << 10) + b + (0x10000 - (0xD800 << 10) - 0xDC00);
      }

      // Strings are null-terminated
      if (codePoint === 0) {
        throw new Error('Cannot encode a string containing the null character');
      }

      // Encode UTF-8
      if (codePoint < 0x80) {
        this.writeByte(codePoint);
      } else {
        if (codePoint < 0x800) {
          this.writeByte(((codePoint >> 6) & 0x1F) | 0xC0);
        } else {
          if (codePoint < 0x10000) {
            this.writeByte(((codePoint >> 12) & 0x0F) | 0xE0);
          } else {
            this.writeByte(((codePoint >> 18) & 0x07) | 0xF0);
            this.writeByte(((codePoint >> 12) & 0x3F) | 0x80);
          }
          this.writeByte(((codePoint >> 6) & 0x3F) | 0x80);
        }
        this.writeByte((codePoint & 0x3F) | 0x80);
      }
    }

    // Strings are null-terminated
    this.writeByte(0);
  };

  kiwi.ByteBuffer = ByteBuffer;
}());

// Parser
(function() {
  var nativeTypes = [
    'bool',
    'byte',
    'float',
    'int',
    'string',
    'uint',
  ];

  // These are special names on the object returned by compileSchema()
  var reservedNames = [
    'ByteBuffer',
    'package',
  ];

  var regex = /((?:-|\b)\d+\b|[=;{}]|\[\]|\b[A-Za-z_][A-Za-z0-9_]*\b|\/\/.*|\s+)/g;
  var identifier = /^[A-Za-z_][A-Za-z0-9_]*$/;
  var whitespace = /^\/\/.*|\s+$/;
  var equals = /^=$/;
  var endOfFile = /^$/;
  var semicolon = /^;$/;
  var integer = /^-?\d+$/;
  var leftBrace = /^\{$/;
  var rightBrace = /^\}$/;
  var arrayToken = /^\[\]$/;
  var enumKeyword = /^enum$/;
  var structKeyword = /^struct$/;
  var messageKeyword = /^message$/;
  var packageKeyword = /^package$/;

  function tokenize(text) {
    var parts = text.split(regex);
    var tokens = [];
    var column = 0;
    var line = 0;

    for (var i = 0; i < parts.length; i++) {
      var part = parts[i];

      // Keep non-whitespace tokens
      if (i & 1) {
        if (!whitespace.test(part)) {
          tokens.push({
            text: part,
            line: line + 1,
            column: column + 1,
          });
        }
      }

      // Detect syntax errors
      else if (part !== '') {
        error('Syntax error ' + quote(part), line + 1, column + 1);
      }

      // Keep track of the line and column counts
      var lines = part.split('\n');
      if (lines.length > 1) column = 0;
      line += lines.length - 1;
      column += lines[lines.length - 1].length;
    }

    // End-of-file token
    tokens.push({
      text: '',
      line: line,
      column: column,
    });

    return tokens;
  }

  function parse(tokens) {
    function current() {
      return tokens[index];
    }

    function eat(test) {
      if (test.test(current().text)) {
        index++;
        return true;
      }
      return false;
    }

    function expect(test, expected) {
      if (!eat(test)) {
        var token = current();
        error('Expected ' + expected + ' but found ' + quote(token.text), token.line, token.column);
      }
    }

    function unexpectedToken() {
      var token = current();
      error('Unexpected token ' + quote(token.text), token.line, token.column);
    }

    var definitions = [];
    var package = null;
    var index = 0;

    if (eat(packageKeyword)) {
      package = current().text;
      expect(identifier, 'identifier');
      expect(semicolon, '";"');
    }

    while (index < tokens.length && !eat(endOfFile)) {
      var fields = [];
      var kind;

      if (eat(enumKeyword)) kind = 'ENUM';
      else if (eat(structKeyword)) kind = 'STRUCT';
      else if (eat(messageKeyword)) kind = 'MESSAGE';
      else unexpectedToken();

      // All definitions start off the same
      var name = current();
      expect(identifier, 'identifier');
      expect(leftBrace, '"{"');

      // Parse fields
      while (!eat(rightBrace)) {
        var type = null;
        var isArray = false;

        // Enums don't have types
        if (kind !== 'ENUM') {
          type = current().text;
          expect(identifier, 'identifier');
          isArray = eat(arrayToken);
        }

        var field = current();
        expect(identifier, 'identifier');

        // Structs don't have explicit values
        var value = null;
        if (kind !== 'STRUCT') {
          expect(equals, '"="');
          value = current();
          expect(integer, 'integer');

          if ((value.text | 0) + '' !== value.text) {
            error('Invalid integer ' + quote(value.text), value.line, value.column);
          }
        }

        expect(semicolon, '";"');

        fields.push({
          name: field.text,
          line: field.line,
          column: field.column,
          type: type,
          isArray: isArray,
          value: value !== null ? value.text | 0 : fields.length + 1,
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
      package: package,
      definitions: definitions,
    };
  }

  function verify(root) {
    var definedTypes = nativeTypes.slice();
    var definitions = {};

    // Define definitions
    for (var i = 0; i < root.definitions.length; i++) {
      var definition = root.definitions[i];
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
    for (var i = 0; i < root.definitions.length; i++) {
      var definition = root.definitions[i];
      var fields = definition.fields;

      if (definition.kind === 'ENUM' || fields.length === 0) {
        continue;
      }

      // Check types
      for (var j = 0; j < fields.length; j++) {
        var field = fields[j];
        if (definedTypes.indexOf(field.type) === -1) {
          error('The type ' + quote(field.type) + ' is not defined for field ' + quote(field.name), field.line, field.column);
        }
      }

      // Check values
      var values = [];
      for (var j = 0; j < fields.length; j++) {
        var field = fields[j];
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
    var state = {};
    var check = function(name) {
      var definition = definitions[name];
      if (definition && definition.kind === 'STRUCT') {
        if (state[name] === 1) {
          error('Recursive nesting of ' + quote(name) + ' is not allowed', definition.line, definition.column);
        }
        if (state[name] !== 2 && definition) {
          state[name] = 1;
          var fields = definition.fields;
          for (var i = 0; i < fields.length; i++) {
            var field = fields[i];
            if (!field.isArray) {
              check(field.type);
            }
          }
          state[name] = 2;
        }
      }
      return true;
    };
    for (var i = 0; i < root.definitions.length; i++) {
      check(root.definitions[i].name);
    }
  }

  function parseSchema(text) {
    var schema = parse(tokenize(text));
    verify(schema);
    return schema;
  }

  kiwi.nativeTypes = nativeTypes;
  kiwi.reservedNames = reservedNames;
  kiwi.parseSchema = parseSchema;
}());

// Binary schema support
(function() {
  var ByteBuffer = kiwi.ByteBuffer;
  var types = ['bool', 'byte', 'int', 'uint', 'float', 'string'];
  var kinds = ['ENUM', 'STRUCT', 'MESSAGE'];

  function decodeBinarySchema(bb) {
    if (!(bb instanceof ByteBuffer)) {
      bb = new ByteBuffer(bb);
    }

    var definitionCount = bb.readVarUint();
    var definitions = [];

    // Read in the schema
    for (var i = 0; i < definitionCount; i++) {
      var definitionName = bb.readString();
      var kind = bb.readByte();
      var fieldCount = bb.readVarUint();
      var fields = [];

      for (var j = 0; j < fieldCount; j++) {
        var fieldName = bb.readString();
        var type = bb.readVarInt();
        var isArray = !!bb.readByte();
        var value = bb.readVarUint();

        fields.push({
          name: fieldName,
          line: 0,
          column: 0,
          type: type,
          isArray: isArray,
          value: value,
        });
      }

      definitions.push({
        name: definitionName,
        line: 0,
        column: 0,
        kind: kinds[kind],
        fields: fields,
      });
    }

    // Bind type names afterwards
    for (var i = 0; i < definitionCount; i++) {
      var fields = definitions[i].fields;
      for (var j = 0; j < fields.length; j++) {
        var field = fields[j];

        if (field.type < 0) {
          if (~field.type >= types.length) {
            throw new Error('Invalid type ' + field.type);
          }
          field.type = types[~field.type];
        }

        else {
          if (field.type >= definitions.length) {
            throw new Error('Invalid type ' + field.type);
          }
          field.type = definitions[field.type].name;
        }
      }
    }

    return {
      package: null,
      definitions: definitions,
    };
  }

  function encodeBinarySchema(schema) {
    schema = convertSchema(schema);

    var bb = new ByteBuffer();
    var definitions = schema.definitions;
    var definitionIndex = {};

    bb.writeVarUint(definitions.length);

    for (var i = 0; i < definitions.length; i++) {
      definitionIndex[definitions[i].name] = i;
    }

    for (var i = 0; i < definitions.length; i++) {
      var definition = definitions[i];

      bb.writeString(definition.name);
      bb.writeByte(kinds.indexOf(definition.kind));
      bb.writeVarUint(definition.fields.length);

      for (var j = 0; j < definition.fields.length; j++) {
        var field = definition.fields[j];
        var type = types.indexOf(field.type);

        bb.writeString(field.name);
        bb.writeVarInt(type === -1 ? definitionIndex[field.type] : ~type);
        bb.writeByte(+field.isArray);
        bb.writeVarUint(field.value);
      }
    }

    return bb.toUint8Array();
  }

  kiwi.decodeBinarySchema = decodeBinarySchema;
  kiwi.encodeBinarySchema = encodeBinarySchema;
}());

// JavaScript Compiler
(function() {
  var ByteBuffer = kiwi.ByteBuffer;

  function compileDecode(definition, definitions) {
    var lines = [];
    var indent = '  ';

    lines.push('function(bb) {');
    lines.push('  var result = {};');
    lines.push('  if (!(bb instanceof this.ByteBuffer)) {');
    lines.push('    bb = new this.ByteBuffer(bb);');
    lines.push('  }');
    lines.push('');

    if (definition.kind === 'MESSAGE') {
      lines.push('  while (true) {');
      lines.push('    switch (bb.readByte()) {');
      lines.push('    case 0:');
      lines.push('      return result;');
      lines.push('');
      indent = '      ';
    }

    for (var i = 0; i < definition.fields.length; i++) {
      var field = definition.fields[i];
      var code;

      switch (field.type) {
        case 'bool': {
          code = '!!bb.readByte()';
          break;
        }

        case 'byte': {
          code = 'bb.readByte()';
          break;
        }

        case 'int': {
          code = 'bb.readVarInt()';
          break;
        }

        case 'uint': {
          code = 'bb.readVarUint()';
          break;
        }

        case 'float': {
          code = 'bb.readVarFloat()';
          break;
        }

        case 'string': {
          code = 'bb.readString()';
          break;
        }

        default: {
          var type = definitions[field.type];
          if (!type) {
            error('Invalid type ' + quote(field.type) + ' for field ' + quote(field.name), field.line, field.column);
          } else if (type.kind === 'ENUM') {
            code = 'this[' + quote(type.name) + '][bb.readVarUint()]';
          } else {
            code = 'this[' + quote('decode' + type.name) + '](bb)';
          }
        }
      }

      if (definition.kind === 'MESSAGE') {
        lines.push('    case ' + field.value + ':');
      }

      if (field.isArray) {
        lines.push(indent + 'var values = result[' + quote(field.name) + '] = [];');
        lines.push(indent + 'var length = bb.readVarUint();');
        lines.push(indent + 'while (length-- > 0) values.push(' + code + ');');
      }

      else {
        lines.push(indent + 'result[' + quote(field.name) + '] = ' + code + ';');
      }

      if (definition.kind === 'MESSAGE') {
        lines.push('      break;');
        lines.push('');
      }
    }

    if (definition.kind === 'MESSAGE') {
      lines.push('    default:');
      lines.push('      throw new Error("Attempted to parse invalid message");');
      lines.push('    }');
      lines.push('  }');
    }

    else {
      lines.push('  return result;');
    }

    lines.push('}');

    return lines.join('\n');
  }

  function compileEncode(definition, definitions) {
    var lines = [];

    lines.push('function(message, bb) {');
    lines.push('  var isTopLevel = !bb;');
    lines.push('  if (isTopLevel) bb = new this.ByteBuffer();');

    for (var j = 0; j < definition.fields.length; j++) {
      var field = definition.fields[j];
      var code;

      switch (field.type) {
        case 'bool': {
          code = 'bb.writeByte(value);';
          break;
        }

        case 'byte': {
          code = 'bb.writeByte(value);';
          break;
        }

        case 'int': {
          code = 'bb.writeVarInt(value);';
          break;
        }

        case 'uint': {
          code = 'bb.writeVarUint(value);';
          break;
        }

        case 'float': {
          code = 'bb.writeVarFloat(value);';
          break;
        }

        case 'string': {
          code = 'bb.writeString(value);';
          break;
        }

        default: {
          var type = definitions[field.type];
          if (!type) {
            throw new Error('Invalid type ' + quote(field.type) + ' for field ' + quote(field.name));
          } else if (type.kind === 'ENUM') {
            code =
              'var encoded = this[' + quote(type.name) + '][value]; ' +
              'if (encoded === void 0) throw new Error("Invalid value " + JSON.stringify(value) + ' + quote(' for enum ' + quote(type.name)) + '); ' +
              'bb.writeVarUint(encoded);';
          } else {
            code = 'this[' + quote('encode' + type.name) + '](value, bb);';
          }
        }
      }

      lines.push('');
      lines.push('  var value = message[' + quote(field.name) + '];');
      lines.push('  if (value != null) {'); // Comparing with null using "!=" also checks for undefined

      if (definition.kind === 'MESSAGE') {
        lines.push('    bb.writeVarUint(' + field.value + ');');
      }

      if (field.isArray) {
        lines.push('    var values = value, n = values.length;');
        lines.push('    bb.writeVarUint(n);');
        lines.push('    for (var i = 0; i < n; i++) {');
        lines.push('      value = values[i];');
        lines.push('      ' + code);
        lines.push('    }');
      }

      else {
        lines.push('    ' + code);
      }

      if (definition.kind === 'STRUCT') {
        lines.push('  } else {');
        lines.push('    throw new Error(' + quote('Missing required field ' + quote(field.name)) + ');');
      }

      lines.push('  }');
    }

    // A field id of zero is reserved to indicate the end of the message
    if (definition.kind === 'MESSAGE') {
      lines.push('  bb.writeVarUint(0);');
    }

    lines.push('');
    lines.push('  if (isTopLevel) return bb.toUint8Array();');
    lines.push('}');

    return lines.join('\n');
  }

  function compileSchemaJS(schema) {
    schema = convertSchema(schema);

    var definitions = {};
    var name = schema.package;
    var js = [];

    if (name !== null) {
      js.push('var ' + name + ' = exports || ' + name + ' || {}, exports;');
    } else {
      js.push('var exports = exports || {};');
      name = 'exports';
    }

    js.push(name + '.ByteBuffer = ' + name + '.ByteBuffer || require("kiwi-schema").ByteBuffer;');

    for (var i = 0; i < schema.definitions.length; i++) {
      var definition = schema.definitions[i];
      definitions[definition.name] = definition;
    }

    for (var i = 0; i < schema.definitions.length; i++) {
      var definition = schema.definitions[i];

      switch (definition.kind) {
        case 'ENUM': {
          var value = {};
          for (var j = 0; j < definition.fields.length; j++) {
            var field = definition.fields[j];
            value[field.name] = field.value;
            value[field.value] = field.name;
          }
          js.push(name + '[' + quote(definition.name) + '] = ' + JSON.stringify(value, null, 2) + ';');
          break;
        }

        case 'STRUCT':
        case 'MESSAGE': {
          js.push('');
          js.push(name + '[' + quote('decode' + definition.name) + '] = ' + compileDecode(definition, definitions) + ';');
          js.push('');
          js.push(name + '[' + quote('encode' + definition.name) + '] = ' + compileEncode(definition, definitions) + ';');
          break;
        }

        default: {
          error('Invalid definition kind ' + quote(definition.kind), definition.line, definition.column);
          break;
        }
      }
    }

    js.push('');
    return js.join('\n');
  }

  function compileSchemaJIT(schema) {
    var result = {
      ByteBuffer: ByteBuffer,
    };
    new Function('exports', compileSchemaJS(schema))(result);
    return result;
  }

  kiwi.compileSchema = compileSchemaJIT;
  kiwi.compileSchemaJS = compileSchemaJS;
}());

// C++ Compiler
(function() {
  var ByteBuffer = kiwi.ByteBuffer;

  function cppType(definitions, field, isArray) {
    var type;

    switch (field.type) {
      case 'bool': type = 'bool'; break;
      case 'byte': type = 'uint8_t'; break;
      case 'int': type = 'int32_t'; break;
      case 'uint': type = 'uint32_t'; break;
      case 'float': type = 'float'; break;
      case 'string': type = 'kiwi::String'; break;

      default: {
        var definition = definitions[field.type];

        if (!definition) {
          error('Invalid type ' + quote(field.type) + ' for field ' + quote(field.name), field.line, field.column);
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

  function cppFieldName(field) {
    return '_data_' + field.name;
  }

  function cppFlagIndex(i) {
    return i >> 5;
  }

  function cppFlagMask(i) {
    return 1 << (i % 32) >>> 0;
  }

  function cppIsFieldPointer(definitions, field) {
    return !field.isArray && field.type in definitions && definitions[field.type].kind !== 'ENUM';
  }

  function compileSchemaCPP(schema) {
    schema = convertSchema(schema);

    var definitions = {};
    var cpp = [];

    cpp.push('#include "kiwi.h"');
    cpp.push('');

    if (schema.package !== null) {
      cpp.push('namespace ' + schema.package + ' {');
      cpp.push('');
    }

    for (var i = 0; i < schema.definitions.length; i++) {
      var definition = schema.definitions[i];
      definitions[definition.name] = definition;
    }

    cpp.push('class BinarySchema {');
    cpp.push('public:');
    cpp.push('  bool parse(kiwi::ByteBuffer &bb);');

    for (var i = 0; i < schema.definitions.length; i++) {
      var definition = schema.definitions[i];
      if (definition.kind === 'MESSAGE') {
        cpp.push('  bool skip' + definition.name + 'Field(kiwi::ByteBuffer &bb, uint32_t id) const;');
      }
    }

    cpp.push('');
    cpp.push('private:');
    cpp.push('  kiwi::BinarySchema _schema;');

    for (var i = 0; i < schema.definitions.length; i++) {
      var definition = schema.definitions[i];
      if (definition.kind === 'MESSAGE') {
        cpp.push('  uint32_t _index' + definition.name + ' = 0;');
      }
    }

    cpp.push('};');
    cpp.push('');

    for (var i = 0; i < schema.definitions.length; i++) {
      var definition = schema.definitions[i];

      if (definition.kind === 'ENUM') {
        cpp.push('enum class ' + definition.name + ' : uint32_t {');
        for (var j = 0; j < definition.fields.length; j++) {
          var field = definition.fields[j];
          cpp.push('  ' + field.name + ' = ' + field.value + ',');
        }
        cpp.push('};');
        cpp.push('');
      }

      else if (definition.kind !== 'STRUCT' && definition.kind !== 'MESSAGE') {
        error('Invalid definition kind ' + quote(definition.kind), definition.line, definition.column);
      }
    }

    for (var pass = 0; pass < 3; pass++) {
      var newline = false;

      if (pass === 2) {
        cpp.push('#ifdef IMPLEMENT_SCHEMA_H');
        cpp.push('');

        cpp.push('bool BinarySchema::parse(kiwi::ByteBuffer &bb) {');
        cpp.push('  if (!_schema.parse(bb)) return false;');

        for (var i = 0; i < schema.definitions.length; i++) {
          var definition = schema.definitions[i];
          if (definition.kind === 'MESSAGE') {
            cpp.push('  if (!_schema.findDefinition("' + definition.name + '", _index' + definition.name + ')) return false;');
          }
        }

        cpp.push('  return true;');
        cpp.push('}');
        cpp.push('');

        for (var i = 0; i < schema.definitions.length; i++) {
          var definition = schema.definitions[i];
          if (definition.kind === 'MESSAGE') {
            cpp.push('bool BinarySchema::skip' + definition.name + 'Field(kiwi::ByteBuffer &bb, uint32_t id) const {');
            cpp.push('  return _schema.skipField(bb, _index' + definition.name + ', id);');
            cpp.push('}');
            cpp.push('');
          }
        }
      }

      for (var i = 0; i < schema.definitions.length; i++) {
        var definition = schema.definitions[i];

        if (definition.kind === 'ENUM') {
          continue;
        }

        var fields = definition.fields;

        if (pass === 0) {
          cpp.push('class ' + definition.name + ';');
          newline = true;
        }

        else if (pass === 1) {
          cpp.push('class ' + definition.name + ' {');
          cpp.push('public:');

          for (var j = 0; j < fields.length; j++) {
            var field = fields[j];
            var name = cppFieldName(field);
            var type = cppType(definitions, field, field.isArray);
            var flagIndex = cppFlagIndex(j);
            var flagMask = cppFlagMask(j);

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

          for (var j = 0; j < fields.length; j++) {
            var field = fields[j];
            var name = cppFieldName(field);
            var type = cppType(definitions, field, field.isArray);

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
          for (var j = 0; j < fields.length; j++) {
            var field = fields[j];
            var name = cppFieldName(field);
            var type = cppType(definitions, field, field.isArray);
            var flagIndex = cppFlagIndex(j);
            var flagMask = cppFlagMask(j);

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

          for (var j = 0; j < fields.length; j++) {
            var field = fields[j];
            var name = cppFieldName(field);
            var value = field.isArray ? '_it' : name;
            var flagIndex = cppFlagIndex(j);
            var flagMask = cppFlagMask(j);
            var code;

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
                var type = definitions[field.type];

                if (!type) {
                  error('Invalid type ' + quote(field.type) + ' for field ' + quote(field.name), field.line, field.column);
                }

                else if (type.kind === 'ENUM') {
                  code = '_bb.writeVarUint(static_cast<uint32_t>(' + value + '));';
                }

                else {
                  code = 'if (!' + value + (cppIsFieldPointer(definitions, field) ? '->' : '.') + 'encode(_bb)) return false;';
                }
              }
            }

            var indent = '  ';
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

          for (var j = 0; j < fields.length; j++) {
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

          for (var j = 0; j < fields.length; j++) {
            var field = fields[j];
            var name = cppFieldName(field);
            var value = field.isArray ? '_it' : name;
            var isPointer = cppIsFieldPointer(definitions, field);
            var code;

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
                var type = definitions[field.type];

                if (!type) {
                  error('Invalid type ' + quote(field.type) + ' for field ' + quote(field.name), field.line, field.column);
                }

                else if (type.kind === 'ENUM') {
                  code = '_bb.readVarUint(reinterpret_cast<uint32_t &>(' + value + '))';
                }

                else {
                  code = value + (isPointer ? '->' : '.') + 'decode(_bb, _pool, _schema)';
                }
              }
            }

            var type = cppType(definitions, field, false);
            var indent = '  ';

            if (definition.kind === 'MESSAGE') {
              cpp.push('      case ' + field.value + ':');
              indent = '        ';
            }

            if (field.isArray) {
              cpp.push(indent + 'if (!_bb.readVarUint(_count)) return false;');
              cpp.push(indent + 'for (' + type + ' &_it : set_' + field.name + '(_pool, _count)) if (!' + code + ') return false;');
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

            if (definition.kind === 'MESSAGE') {
              cpp.push('        break;');
            }
          }

          if (definition.kind === 'MESSAGE') {
            cpp.push('      default:');
            cpp.push('        if (!_schema || !_schema->skip' + definition.name + 'Field(_bb, _type)) return false;');
            cpp.push('        break;');
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

  kiwi.compileSchemaCPP = compileSchemaCPP;
}());

// Skew Compiler
(function() {
  var ByteBuffer = kiwi.ByteBuffer;

  function popTrailingNewline(lines) {
    if (lines[lines.length - 1] === '') {
      lines.pop();
    }
  }

  function skewDefaultValueForField(definitions, field) {
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
    }

    var def = definitions[field.type];

    if (def.kind === 'ENUM') {
      if (def.fields.length > 0) {
        return '.' + def.fields[0].name;
      }
      return '0 as ' + field.type;
    }

    return 'null';
  }

  function skewTypeForField(field) {
    var type;

    switch (field.type) {
      case 'bool': type = 'bool'; break;
      case 'byte':
      case 'int':
      case 'uint': type = 'int'; break;
      case 'float': type = 'double'; break;
      case 'string': type = 'string'; break;
      default: type = field.type; break;
    }

    if (field.isArray) {
      type = 'List<' + type + '>';
    }

    return type;
  }

  function compileSchemaSkew(schema) {
    schema = convertSchema(schema);

    var definitions = {};
    var indent = '';
    var lines = [];

    if (schema.package !== null) {
      lines.push('namespace ' + schema.package + ' {');
      indent += '  ';
    }

    for (var i = 0; i < schema.definitions.length; i++) {
      var definition = schema.definitions[i];
      definitions[definition.name] = definition;
    }

    lines.push(indent + 'class BinarySchema {');
    lines.push(indent + '  var _schema = Kiwi.BinarySchema.new');

    for (var i = 0; i < schema.definitions.length; i++) {
      var definition = schema.definitions[i];
      if (definition.kind === 'MESSAGE') {
        lines.push(indent + '  var _index' + definition.name + ' = 0');
      }
    }

    lines.push('');
    lines.push(indent + '  def parse(bytes Uint8Array) {');
    lines.push(indent + '    _schema.parse(Kiwi.ByteBuffer.new(bytes))');

    for (var i = 0; i < schema.definitions.length; i++) {
      var definition = schema.definitions[i];
      if (definition.kind === 'MESSAGE') {
        lines.push(indent + '    _index' + definition.name + ' = _schema.findDefinition("' + definition.name + '")');
      }
    }

    lines.push(indent + '  }');

    for (var i = 0; i < schema.definitions.length; i++) {
      var definition = schema.definitions[i];
      if (definition.kind === 'MESSAGE') {
      lines.push('');
        lines.push(indent + '  def skip' + definition.name + 'Field(bb Kiwi.ByteBuffer, id int) {');
        lines.push(indent + '    _schema.skipField(bb, _index' + definition.name + ', id)');
        lines.push(indent + '  }');
      }
    }

    lines.push(indent + '}');
    lines.push('');

    for (var i = 0; i < schema.definitions.length; i++) {
      var definition = schema.definitions[i];

      switch (definition.kind) {
        case 'ENUM': {
          var encode = {};
          var decode = {};

          lines.push(indent + 'enum ' + definition.name + ' {');
          for (var j = 0; j < definition.fields.length; j++) {
            var field = definition.fields[j];
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

          for (var j = 0; j < definition.fields.length; j += 32) {
            lines.push(indent + '  var _flags' + (j >> 5) + ' = 0');
          }

          for (var j = 0; j < definition.fields.length; j++) {
            var field = definition.fields[j];
            lines.push(indent + '  var _' + field.name + ' ' + skewTypeForField(field) + ' = ' + skewDefaultValueForField(definitions, field));
          }

          lines.push('');

          for (var j = 0; j < definition.fields.length; j++) {
            var field = definition.fields[j];
            var type = skewTypeForField(field);
            var flags = '_flags' + (j >> 5);
            var mask = '' + (1 << (j % 31));

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

          for (var j = 0; j < definition.fields.length; j++) {
            var field = definition.fields[j];
            var value = '_' + field.name;
            var code;

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

              default: {
                var type = definitions[field.type];
                if (!type) {
                  throw new Error('Invalid type ' + quote(field.type) + ' for field ' + quote(field.name));
                } else if (type.kind === 'ENUM') {
                  code = 'bb.writeVarUint(' + type.name + '.encode(' + value + '))';
                } else {
                  code = value + '.encode(bb)';
                }
              }
            }

            var nestedIndent = indent + '    ';

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

          for (var j = 0; j < definition.fields.length; j++) {
            if (definition.fields[j].isArray) {
              lines.push(indent + '    var count = 0');
              break;
            }
          }

          var nestedIndent = indent + '  ';

          if (definition.kind === 'MESSAGE') {
            lines.push(indent + '    while true {');
            lines.push(indent + '      var type = bb.readByte');
            lines.push(indent + '      switch type {');
            lines.push(indent + '        case 0 {');
            lines.push(indent + '          break');
            lines.push(indent + '        }');
            lines.push('');
            nestedIndent += '      ';
          }

          for (var j = 0; j < definition.fields.length; j++) {
            var field = definition.fields[j];
            var code;

            switch (field.type) {
              case 'bool': {
                code = 'bb.readByte as bool';
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

              default: {
                var type = definitions[field.type];
                if (!type) {
                  throw new Error('Invalid type ' + quote(field.type) + ' for field ' + quote(field.name));
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
              lines.push(nestedIndent + '  count = bb.readVarUint');
              lines.push(nestedIndent + '  self.' + field.name + ' = []');
              lines.push(nestedIndent + '  for array = self._' + field.name + '; count != 0; count-- {');
              lines.push(nestedIndent + '    array.append(' + code + ')');
              lines.push(nestedIndent + '  }');
            } else {
              lines.push(nestedIndent + '  self.' + field.name + ' = ' + code);
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

  kiwi.compileSchemaSkew = compileSchemaSkew;
}());

// TypeScript Compiler
(function() {
  var ByteBuffer = kiwi.ByteBuffer;

  function compileSchemaTypeScript(schema) {
    schema = convertSchema(schema);

    var isFirst = true;
    var indent = '';
    var lines = [];

    if (schema.package !== null) {
      lines.push('export namespace ' + schema.package + ' {');
      indent += '  ';
    }

    for (var i = 0; i < schema.definitions.length; i++) {
      var definition = schema.definitions[i];

      if (definition.kind === 'ENUM') {
        if (isFirst) isFirst = false;
        else lines.push('');
        lines.push(indent + 'export type ' + definition.name + ' =');

        for (var j = 0; j < definition.fields.length; j++) {
          lines.push(indent + '  ' + JSON.stringify(definition.fields[j].name) + (j + 1 < definition.fields.length ? ' | ' : ';'));
        }

        if (!definition.fields.length) {
          lines.push(indent + '  any;');
        }
      }
    }

    for (var i = 0; i < schema.definitions.length; i++) {
      var definition = schema.definitions[i];

      if (definition.kind === 'STRUCT' || definition.kind === 'MESSAGE') {
        if (isFirst) isFirst = false;
        else lines.push('');

        lines.push(indent + 'export interface ' + definition.name + ' {');

        for (var j = 0; j < definition.fields.length; j++) {
          var field = definition.fields[j];
          var type;

          switch (field.type) {
            case 'bool': type = 'boolean'; break;
            case 'byte': case 'int': case 'uint': case 'float': type = 'number'; break;
            default: type = field.type; break;
          }

          lines.push(indent + '  ' + field.name + (definition.kind === 'MESSAGE' ? '?' : '') + ': ' + type + (field.isArray ? '[]' : '') + ';');
        }

        lines.push(indent + '}');
      }

      else if (definition.kind !== 'ENUM') {
        error('Invalid definition kind ' + quote(definition.kind), definition.line, definition.column);
      }
    }

    if (schema.package !== null) {
      lines.push('}');
    }

    lines.push('');
    return lines.join('\n');
  }

  kiwi.compileSchemaTypeScript = compileSchemaTypeScript;
}());

}());
