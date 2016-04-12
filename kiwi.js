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

// ByteBuffer
(function() {
  var uint8 = new Uint8Array(4);
  var float32 = new Float32Array(uint8.buffer);
  var utf8 = new ByteBuffer(); // A pre-allocated accumulation buffer to avoid extra GC

  function ByteBuffer(data) {
    if (data && !(data instanceof Uint8Array)) {
      throw new Error('Must initialize a ByteBuffer with a Uint8Array');
    }
    this._data = data || new Uint8Array(256);
    this._index = 0;
    this.length = 0;
  }

  ByteBuffer.prototype.toUint8Array = function() {
    return this._data.subarray(0, this.length);
  };

  ByteBuffer.prototype.seekTo = function(index) {
    if (index > this._data.length) {
      throw new Error('Index out of bounds');
    }
    this._index = index;
  };

  ByteBuffer.prototype.readByte = function() {
    if (this._index + 1 > this._data.length) {
      throw new Error('Index out of bounds');
    }
    return this._data[this._index++];
  };

  ByteBuffer.prototype.readFloat = function() {
    var index = this._index;
    var data = this._data;
    if (index + 4 > data.length) {
      throw new Error('Index out of bounds');
    }
    uint8[0] = data[index];
    uint8[1] = data[index + 1];
    uint8[2] = data[index + 2];
    uint8[3] = data[index + 3];
    this._index = index + 4;
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
    var length = this.readVarUint();
    var result = '';

    while (length > 0) {
      var codePoint;

      // Decode UTF-8
      var a = this.readByte();
      length--;
      if (a < 0xC0 || length === 0) {
        codePoint = a;
      } else {
        var b = this.readByte();
        length--;
        if (a < 0xE0 || length === 0) {
          codePoint = ((a & 0x1F) << 6) | (b & 0x3F);
        } else {
          var c = this.readByte();
          length--;
          if (a < 0xF0 || length === 0) {
            codePoint = ((a & 0x0F) << 12) | ((b & 0x3F) << 6) | (c & 0x3F);
          } else {
            var d = this.readByte();
            length--;
            codePoint = ((a & 0x07) << 18) | ((b & 0x3F) << 12) | ((c & 0x3F) << 6) | (d & 0x3F);
          }
        }
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

  ByteBuffer.prototype.writeFloat = function(value) {
    var index = this.length;
    this._growBy(4);
    var data = this._data;
    float32[0] = value;
    data[index] = uint8[0];
    data[index + 1] = uint8[1];
    data[index + 2] = uint8[2];
    data[index + 3] = uint8[3];
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
    utf8.length = 0;

    for (var i = 0; i < value.length; i++) {
      // Decode UTF-16
      var a = value.charCodeAt(i);
      if (i + 1 === value.length || a < 0xD800 || a >= 0xDC00) {
        codePoint = a;
      } else {
        var b = value.charCodeAt(++i);
        codePoint = (a << 10) + b + (0x10000 - (0xD800 << 10) - 0xDC00);
      }

      // Encode UTF-8
      if (codePoint < 0x80) {
        utf8.writeByte(codePoint);
      } else {
        if (codePoint < 0x800) {
          utf8.writeByte(((codePoint >> 6) & 0x1F) | 0xC0);
        } else {
          if (codePoint < 0x10000) {
            utf8.writeByte(((codePoint >> 12) & 0x0F) | 0xE0);
          } else {
            utf8.writeByte(((codePoint >> 18) & 0x07) | 0xF0);
            utf8.writeByte(((codePoint >> 12) & 0x3F) | 0x80);
          }
          utf8.writeByte(((codePoint >> 6) & 0x3F) | 0x80);
        }
        utf8.writeByte((codePoint & 0x3F) | 0x80);
      }
    }

    // Append all bytes once we know how many there are
    this.writeVarUint(utf8.length);
    var index = this.length;
    this._growBy(utf8.length);
    this._data.set((index + utf8._data.length > this._data.length ? utf8.toUint8Array() : utf8._data), index);
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
  var requiredKeyword = /^required$/;

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
        var isRequired = kind === 'STRUCT' || eat(requiredKeyword);
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
          isRequired: isRequired,
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
      if (state[name] === 1) {
        error('Recursive nesting of ' + quote(name) + ' is not allowed', definition.line, definition.column);
      }
      if (state[name] !== 2 && definition) {
        state[name] = 1;
        var fields = definition.fields;
        for (var i = 0; i < fields.length; i++) {
          check(fields[i].type);
        }
        state[name] = 2;
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

// JavaScript Compiler
(function() {
  var ByteBuffer = kiwi.ByteBuffer;

  function compileDecodeJS(definition, definitions) {
    var lines = [];
    var indent = '';

    lines.push('function(bb) {');
    lines.push('  var result = {};');
    lines.push('  if (!(bb instanceof this.ByteBuffer)) {');
    lines.push('    bb = new this.ByteBuffer(bb);');
    lines.push('  }');

    if (definition.kind === 'MESSAGE') {
      lines.push('  while (true) {');
      lines.push('    switch (bb.readByte()) {');
      lines.push('    case 0:');
      lines.push('      return result;');
      indent = '      ';
    }

    for (var j = 0; j < definition.fields.length; j++) {
      var field = definition.fields[j];
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
          code = 'bb.readFloat()';
          break;
        }

        case 'string': {
          code = 'bb.readString()';
          break;
        }

        default: {
          var type = definitions[field.type];
          if (!type) {
            throw new Error('Invalid type ' + quote(field.type) + ' for field ' + quote(field.name));
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

  function compileEncodeJS(definition, definitions) {
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
          code = 'bb.writeFloat(value);';
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

      if (field.isRequired) {
        lines.push('  } else {');
        lines.push('    throw new Error(' + quote('Missing required field ' + quote(field.name)) + ');');
      }

      lines.push('  }');
    }

    // A field id of zero is reserved to indicate the end of the message
    if (definition.kind === 'MESSAGE') {
      lines.push('  bb.writeVarUint(0);');
    }

    lines.push('  if (isTopLevel) return bb.toUint8Array();');
    lines.push('}');

    return lines.join('\n');
  }

  function compileSchemaJS(schema) {
    if (typeof schema === 'string') {
      schema = kiwi.parseSchema(schema);
    }

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
          js.push(name + '[' + quote('decode' + definition.name) + '] = ' + compileDecodeJS(definition, definitions) + ';');
          js.push(name + '[' + quote('encode' + definition.name) + '] = ' + compileEncodeJS(definition, definitions) + ';');
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

  function toUpperCase(text) {
    return text.replace(/([a-z])([A-Z])/g, function(_, a, b) { return a + '_' + b; }).toUpperCase();
  }

  function compileDecodeCPP(definition, definitions, pass, lines) {
    var name = definition.name + 'Decoder';

    if (pass === 0) {
      lines.push('class ' + name + ';');
    }

    else if (pass === 1) {
      lines.push('class ' + name + ' : public kiwi::Codec {');
      lines.push('public:');
      lines.push('  ' + name + '() {}');
      lines.push('  ' + name + '(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}');
      lines.push('  ' + name + '(' + name + ' &&decoder) : kiwi::Codec(std::move(decoder)) {}');

      if (definition.kind === 'MESSAGE') {
        lines.push('  bool nextField(int32_t &value);');
        lines.push('  enum {');
        for (var j = 0; j < definition.fields.length; j++) {
          var field = definition.fields[j];
          lines.push('    ' + toUpperCase(field.name) + ' = ' + field.value + ',');
        }
        lines.push('  };');
      }

      for (var j = 0; j < definition.fields.length; j++) {
        var field = definition.fields[j];

        if (field.isArray) {
          lines.push('  bool begin_' + field.name + '(uint32_t &count);');
        }

        switch (field.type) {
          case 'bool': {
            lines.push('  bool read_' + field.name + '(bool &value);');
            break;
          }

          case 'byte': {
            lines.push('  bool read_' + field.name + '(uint8_t &value);');
            break;
          }

          case 'int': {
            lines.push('  bool read_' + field.name + '(int32_t &value);');
            break;
          }

          case 'uint': {
            lines.push('  bool read_' + field.name + '(uint32_t &value);');
            break;
          }

          case 'float': {
            lines.push('  bool read_' + field.name + '(float &value);');
            break;
          }

          case 'string': {
            lines.push('  bool read_' + field.name + '(std::string &value);');
            break;
          }

          default: {
            var type = definitions[field.type];

            if (!type) {
              throw new Error('Invalid type ' + quote(field.type) + ' for field ' + quote(field.name));
            }

            else if (type.kind === 'ENUM') {
              lines.push('  bool read_' + field.name + '(' + type.name + ' &value);');
            }

            else {
              lines.push('  bool read_' + field.name + '(' + type.name + 'Decoder &decoder);');
            }
          }
        }

        if (field.isArray) {
          lines.push('  bool end_' + field.name + '();');
        }
      }

      lines.push('};');
      lines.push('');
    }

    else if (pass === 2) {
      if (definition.kind === 'STRUCT') {
        for (var j = 0; j < definition.fields.length; j++) {
          var field = definition.fields[j];

          if (field.isArray) {
            lines.push('bool ' + name + '::begin_' + field.name + '(uint32_t &count) {');
            lines.push('  assert(false); // TODO');
            lines.push('  return true;');
            lines.push('}');
            lines.push('');
          }

          switch (field.type) {
            case 'bool': {
              lines.push('bool ' + name + '::read_' + field.name + '(bool &value) {');
              lines.push('  uint8_t byte;');
              lines.push('  if (!_structReadByte(' + j + ', byte)) return false;');
              lines.push('  value = byte;');
              lines.push('  return true;');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'byte': {
              lines.push('bool ' + name + '::read_' + field.name + '(uint8_t &value) {');
              lines.push('  return _structReadByte(' + j + ', value);');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'int': {
              lines.push('bool ' + name + '::read_' + field.name + '(int32_t &value) {');
              lines.push('  return _structReadVarInt(' + j + ', value);');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'uint': {
              lines.push('bool ' + name + '::read_' + field.name + '(uint32_t &value) {');
              lines.push('  return _structReadVarUint(' + j + ', value);');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'float': {
              lines.push('bool ' + name + '::read_' + field.name + '(float &value) {');
              lines.push('  return _structReadFloat(' + j + ', value);');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'string': {
              lines.push('bool ' + name + '::read_' + field.name + '(std::string &value) {');
              lines.push('  return _structReadString(' + j + ', value);');
              lines.push('}');
              lines.push('');
              break;
            }

            default: {
              var type = definitions[field.type];

              if (!type) {
                throw new Error('Invalid type ' + quote(field.type) + ' for field ' + quote(field.name));
              }

              else if (type.kind === 'ENUM') {
                lines.push('bool ' + name + '::read_' + field.name + '(' + type.name + ' &value) {');
                lines.push('  uint32_t uint;');
                lines.push('  if (!_structReadVarUint(' + j + ', uint)) return false;');
                lines.push('  value = (' + type.name + ')uint;');
                lines.push('  return true;');
                lines.push('}');
                lines.push('');
              }

              else {
                lines.push('bool ' + name + '::read_' + field.name + '(' + type.name + 'Decoder &decoder) {');
                lines.push('  if (!_bb) return false;');
                lines.push('  _structReadFieldNested(' + j + ', decoder);');
                lines.push('  return true;');
                lines.push('}');
                lines.push('');
              }
            }
          }

          if (field.isArray) {
            lines.push('bool ' + name + '::end_' + field.name + '() {');
            lines.push('  assert(!_countRemaining);');
            lines.push('  return true;');
            lines.push('}');
            lines.push('');
          }
        }
      }

      if (definition.kind === 'MESSAGE') {
        lines.push('bool ' + name + '::nextField(int32_t &value) {');
        lines.push('  if (!_messageReadField()) return false;');
        lines.push('  value = _nextField;');
        lines.push('  return true;');
        lines.push('}');
        lines.push('');

        for (var j = 0; j < definition.fields.length; j++) {
          var field = definition.fields[j];

          if (field.isArray) {
            lines.push('bool ' + name + '::begin_' + field.name + '(uint32_t &count) {');
            lines.push('  assert(false); // TODO');
            lines.push('  return true;');
            lines.push('}');
            lines.push('');
          }

          switch (field.type) {
            case 'bool': {
              lines.push('bool ' + name + '::read_' + field.name + '(bool &value) {');
              lines.push('  uint8_t byte;');
              lines.push('  if (!_messageReadByte(' + field.value + ', byte)) return false;');
              lines.push('  value = byte;');
              lines.push('  return true;');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'byte': {
              lines.push('bool ' + name + '::read_' + field.name + '(uint8_t &value) {');
              lines.push('  return _messageReadByte(' + field.value + ', value);');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'int': {
              lines.push('bool ' + name + '::read_' + field.name + '(int32_t &value) {');
              lines.push('  return _messageReadVarInt(' + field.value + ', value);');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'uint': {
              lines.push('bool ' + name + '::read_' + field.name + '(uint32_t &value) {');
              lines.push('  return _messageReadVarUint(' + field.value + ', value);');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'float': {
              lines.push('bool ' + name + '::read_' + field.name + '(float &value) {');
              lines.push('  return _messageReadFloat(' + field.value + ', value);');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'string': {
              lines.push('bool ' + name + '::read_' + field.name + '(std::string &value) {');
              lines.push('  return _messageReadString(' + field.value + ', value);');
              lines.push('}');
              lines.push('');
              break;
            }

            default: {
              var type = definitions[field.type];

              if (!type) {
                throw new Error('Invalid type ' + quote(field.type) + ' for field ' + quote(field.name));
              }

              else if (type.kind === 'ENUM') {
                lines.push('bool ' + name + '::read_' + field.name + '(' + type.name + ' &value) {');
                lines.push('  uint32_t uint;');
                lines.push('  if (!_messageReadVarUint(' + field.value + ', uint)) return false;');
                lines.push('  value = (' + type.name + ')uint;');
                lines.push('  return true;');
                lines.push('}');
                lines.push('');
              }

              else {
                lines.push('bool ' + name + '::read_' + field.name + '(' + type.name + 'Decoder &decoder) {');
                lines.push('  assert(false); // TODO');
                lines.push('  return false;');
                lines.push('}');
                lines.push('');
              }
            }
          }
        }
      }
    }
  }

  function compileEncodeCPP(definition, definitions, pass, lines) {
    var name = definition.name + 'Encoder';

    if (pass === 0) {
      lines.push('class ' + name + ';');
    }

    else if (pass === 1) {
      lines.push('class ' + name + ' : public kiwi::Codec {');
      lines.push('public:');
      lines.push('  ' + name + '(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}');
      lines.push('  ' + name + '(' + name + ' &&encoder) : kiwi::Codec(std::move(encoder)) {}');
      lines.push('  ~' + name + '();');

      for (var j = 0; j < definition.fields.length; j++) {
        var field = definition.fields[j];

        if (field.isArray) {
          lines.push('  ' + name + ' &begin_' + field.name + '(uint32_t count);');
        }

        switch (field.type) {
          case 'bool': {
            lines.push('  ' + name + ' &add_' + field.name + '(bool value);');
            break;
          }

          case 'byte': {
            lines.push('  ' + name + ' &add_' + field.name + '(uint8_t value);');
            break;
          }

          case 'int': {
            lines.push('  ' + name + ' &add_' + field.name + '(int32_t value);');
            break;
          }

          case 'uint': {
            lines.push('  ' + name + ' &add_' + field.name + '(uint32_t value);');
            break;
          }

          case 'float': {
            lines.push('  ' + name + ' &add_' + field.name + '(float value);');
            break;
          }

          case 'string': {
            lines.push('  ' + name + ' &add_' + field.name + '(const std::string &value);');
            break;
          }

          default: {
            var type = definitions[field.type];

            if (!type) {
              throw new Error('Invalid type ' + quote(field.type) + ' for field ' + quote(field.name));
            }

            else if (type.kind === 'ENUM') {
              lines.push('  ' + name + ' &add_' + field.name + '(' + type.name + ' value);');
            }

            else {
              lines.push('  ' + type.name + 'Encoder add_' + field.name + '();');
            }
          }
        }

        if (field.isArray) {
          lines.push('  ' + name + ' &end_' + field.name + '();');
        }
      }

      if (definition.kind === 'MESSAGE') {
        lines.push('  ' + name + ' &finish();');
      }

      lines.push('};');
      lines.push('');
    }

    else if (pass === 2) {
      if (definition.kind === 'STRUCT') {
        for (var j = 0; j < definition.fields.length; j++) {
          var field = definition.fields[j];
          var prefix = '_structWrite';

          if (field.isArray) {
            lines.push(name + ' &' + name + '::begin_' + field.name + '(uint32_t count) {');
            lines.push('  _structWriteArrayBegin(' + j + ', count);');
            lines.push('  return *this;');
            lines.push('}');
            lines.push('');
            prefix = '_structWriteArray';
          }

          switch (field.type) {
            case 'bool': {
              lines.push(name + ' &' + name + '::add_' + field.name + '(bool value) {');
              lines.push('  ' + prefix + 'Byte(' + j + ', value);');
              lines.push('  return *this;');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'byte': {
              lines.push(name + ' &' + name + '::add_' + field.name + '(uint8_t value) {');
              lines.push('  ' + prefix + 'Byte(' + j + ', value);');
              lines.push('  return *this;');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'int': {
              lines.push(name + ' &' + name + '::add_' + field.name + '(int32_t value) {');
              lines.push('  ' + prefix + 'VarInt(' + j + ', value);');
              lines.push('  return *this;');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'uint': {
              lines.push(name + ' &' + name + '::add_' + field.name + '(uint32_t value) {');
              lines.push('  ' + prefix + 'VarUint(' + j + ', value);');
              lines.push('  return *this;');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'float': {
              lines.push(name + ' &' + name + '::add_' + field.name + '(float value) {');
              lines.push('  ' + prefix + 'Float(' + j + ', value);');
              lines.push('  return *this;');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'string': {
              lines.push(name + ' &' + name + '::add_' + field.name + '(const std::string &value) {');
              lines.push('  ' + prefix + 'String(' + j + ', value);');
              lines.push('  return *this;');
              lines.push('}');
              lines.push('');
              break;
            }

            default: {
              var type = definitions[field.type];

              if (!type) {
                throw new Error('Invalid type ' + quote(field.type) + ' for field ' + quote(field.name));
              }

              else if (type.kind === 'ENUM') {
                lines.push(name + ' &' + name + '::add_' + field.name + '(' + type.name + ' value) {');
                lines.push('  ' + prefix + 'VarUint(' + j + ', (uint32_t)value);');
                lines.push('  return *this;');
                lines.push('}');
                lines.push('');
              }

              else {
                lines.push(type.name + 'Encoder ' + name + '::add_' + field.name + '() {');
                lines.push('  ' + prefix + 'Field(' + j + ');');
                lines.push('  return ' + type.name + 'Encoder(*_bb);');
                lines.push('}');
                lines.push('');
              }
            }
          }

          if (field.isArray) {
            lines.push(name + ' &' + name + '::end_' + field.name + '() {');
            lines.push('  _structWriteArrayEnd(' + j + ');');
            lines.push('  return *this;');
            lines.push('}');
            lines.push('');
          }
        }

        lines.push(name + '::~' + name + '() {');
        lines.push('  assert(!_bb || _nextField++ == ' + definition.fields.length + '); // Must set all fields');
        lines.push('}');
        lines.push('');
      }

      else if (definition.kind === 'MESSAGE') {
        for (var j = 0; j < definition.fields.length; j++) {
          var field = definition.fields[j];
          var prefix = '_messageWrite';
          var first = field.value + ', ';

          if (field.isArray) {
            lines.push(name + '& ' + name + '::begin_' + field.name + '(uint32_t count) {');
            lines.push('  _messageWriteArrayBegin(' + first + 'count);');
            lines.push('  return *this;');
            lines.push('}');
            lines.push('');
            prefix = '_messageWriteArray';
            first = '';
          }

          switch (field.type) {
            case 'bool': {
              lines.push(name + '& ' + name + '::add_' + field.name + '(bool value) {');
              lines.push('  ' + prefix + 'Byte(' + first + 'value);');
              lines.push('  return *this;');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'byte': {
              lines.push(name + '& ' + name + '::add_' + field.name + '(uint8_t value) {');
              lines.push('  ' + prefix + 'Byte(' + first + 'value);');
              lines.push('  return *this;');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'int': {
              lines.push(name + '& ' + name + '::add_' + field.name + '(int32_t value) {');
              lines.push('  ' + prefix + 'VarInt(' + first + 'value);');
              lines.push('  return *this;');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'uint': {
              lines.push(name + '& ' + name + '::add_' + field.name + '(uint32_t value) {');
              lines.push('  ' + prefix + 'VarUint(' + first + 'value);');
              lines.push('  return *this;');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'float': {
              lines.push(name + '& ' + name + '::add_' + field.name + '(float value) {');
              lines.push('  ' + prefix + 'Float(' + first + 'value);');
              lines.push('  return *this;');
              lines.push('}');
              lines.push('');
              break;
            }

            case 'string': {
              lines.push(name + '& ' + name + '::add_' + field.name + '(const std::string &value) {');
              lines.push('  ' + prefix + 'String(' + first + 'value);');
              lines.push('  return *this;');
              lines.push('}');
              lines.push('');
              break;
            }

            default: {
              var type = definitions[field.type];

              if (!type) {
                throw new Error('Invalid type ' + quote(field.type) + ' for field ' + quote(field.name));
              }

              else if (type.kind === 'ENUM') {
                lines.push(name + '& ' + name + '::add_' + field.name + '(' + type.name + ' value) {');
                lines.push('  ' + prefix + 'VarUint(' + first + '(uint32_t)value);');
                lines.push('  return *this;');
                lines.push('}');
                lines.push('');
              }

              else {
                lines.push(type.name + 'Encoder ' + name + '::add_' + field.name + '() {');
                lines.push('  ' + prefix + 'Field(' + (field.isArray ? '' : field.value) + ');');
                lines.push('  return ' + type.name + 'Encoder(*_bb);');
                lines.push('}');
                lines.push('');
              }
            }
          }

          if (field.isArray) {
            lines.push(name + ' &' + name + '::end_' + field.name + '() {');
            lines.push('  _messageWriteArrayEnd();');
            lines.push('  return *this;');
            lines.push('}');
            lines.push('');
          }
        }

        lines.push(name + ' &' + name + '::finish() {');
        lines.push('  _messageFinish();');
        lines.push('  return *this;');
        lines.push('}');
        lines.push('');

        lines.push(name + '::~' + name + '() {');
        lines.push('  assert(!_bb); // Must call finish()');
        lines.push('}');
        lines.push('');
      }
    }
  }

  function compileSchemaCPP(schema) {
    if (typeof schema === 'string') {
      schema = kiwi.parseSchema(schema);
    }

    var definitions = {};
    var name = schema.package;
    var cpp = [];

    cpp.push('#include "kiwi.h"');
    cpp.push('');

    if (name !== null) {
      cpp.push('namespace ' + name + ' {');
      cpp.push('');
    }

    for (var i = 0; i < schema.definitions.length; i++) {
      var definition = schema.definitions[i];
      definitions[definition.name] = definition;
    }

    for (var i = 0; i < schema.definitions.length; i++) {
      var definition = schema.definitions[i];

      switch (definition.kind) {
        case 'ENUM': {
          cpp.push('enum class ' + definition.name + ' {');
          for (var j = 0; j < definition.fields.length; j++) {
            var field = definition.fields[j];
            cpp.push('  ' + field.name + ' = ' + field.value + ',');
          }
          cpp.push('};');
          cpp.push('');
          break;
        }

        case 'STRUCT':
        case 'MESSAGE': {
          break;
        }

        default: {
          error('Invalid definition kind ' + quote(definition.kind), definition.line, definition.column);
          break;
        }
      }
    }

    for (var pass = 0; pass < 3; pass++) {
      var newline = false;

      for (var i = 0; i < schema.definitions.length; i++) {
        var definition = schema.definitions[i];

        switch (definition.kind) {
          case 'ENUM': {
            break;
          }

          case 'STRUCT':
          case 'MESSAGE': {
            compileDecodeCPP(definition, definitions, pass, cpp);
            compileEncodeCPP(definition, definitions, pass, cpp);
            if (pass === 0) newline = true;
            break;
          }

          default: {
            error('Invalid definition kind ' + quote(definition.kind), definition.line, definition.column);
            break;
          }
        }
      }

      if (newline) cpp.push('');
    }

    if (name !== null) {
      cpp.push('}');
      cpp.push('');
    }

    return cpp.join('\n');
  }

  kiwi.compileSchemaCPP = compileSchemaCPP;
}());

}());
