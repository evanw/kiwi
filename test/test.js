var assert = require('assert');
var kiwi = require(__dirname + '/../js/kiwi');
var fs = require('fs');
var it = require('node:test');

var schemaText = fs.readFileSync(__dirname + '/test-schema.kiwi', 'utf8');
var schema = kiwi.compileSchema(kiwi.parseSchema(schemaText));

it('struct bool', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeBoolStruct({ x: i })), Buffer.from(o));
    assert.deepEqual(schema.decodeBoolStruct(new Uint8Array(o)), { x: i });
  }

  check(false, [0]);
  check(true, [1]);
});

it('struct byte', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeByteStruct({ x: i })), Buffer.from(o));
    assert.deepEqual(schema.decodeByteStruct(new Uint8Array(o)), { x: i });
  }

  check(0x00, [0x00]);
  check(0x01, [0x01]);
  check(0x7F, [0x7F]);
  check(0x80, [0x80]);
  check(0xFF, [0xFF]);
});

it('struct uint', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeUintStruct({ x: i })), Buffer.from(o));
    assert.deepEqual(schema.decodeUintStruct(new Uint8Array(o)), { x: i });
  }

  check(0x00, [0x00]);
  check(0x01, [0x01]);
  check(0x02, [0x02]);
  check(0x7F, [0x7F]);
  check(0x80, [0x80, 0x01]);
  check(0x81, [0x81, 0x01]);
  check(0x100, [0x80, 0x02]);
  check(0x101, [0x81, 0x02]);
  check(0x17F, [0xFF, 0x02]);
  check(0x180, [0x80, 0x03]);
  check(0x1FF, [0xFF, 0x03]);
  check(0x200, [0x80, 0x04]);
  check(0x7FFF, [0xFF, 0xFF, 0x01]);
  check(0x8000, [0x80, 0x80, 0x02]);
  check(0x7FFFFFFF, [0xFF, 0xFF, 0xFF, 0xFF, 0x07]);
  check(0x80000000, [0x80, 0x80, 0x80, 0x80, 0x08]);
});

it('struct int', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeIntStruct({ x: i })), Buffer.from(o));
    assert.deepEqual(schema.decodeIntStruct(new Uint8Array(o)), { x: i });
  }

  check(0x00, [0x00]);
  check(-0x01, [0x01]);
  check(0x01, [0x02]);
  check(-0x02, [0x03]);
  check(0x02, [0x04]);
  check(-0x3F, [0x7D]);
  check(0x3F, [0x7E]);
  check(-0x40, [0x7F]);
  check(0x40, [0x80, 0x01]);
  check(-0x3FFF, [0xFD, 0xFF, 0x01]);
  check(0x3FFF, [0xFE, 0xFF, 0x01]);
  check(-0x4000, [0xFF, 0xFF, 0x01]);
  check(0x4000, [0x80, 0x80, 0x02]);
  check(-0x3FFFFFFF, [0xFD, 0xFF, 0xFF, 0xFF, 0x07]);
  check(0x3FFFFFFF, [0xFE, 0xFF, 0xFF, 0xFF, 0x07]);
  check(-0x40000000, [0xFF, 0xFF, 0xFF, 0xFF, 0x07]);
  check(0x40000000, [0x80, 0x80, 0x80, 0x80, 0x08]);
  check(-0x7FFFFFFF, [0xFD, 0xFF, 0xFF, 0xFF, 0x0F]);
  check(0x7FFFFFFF, [0xFE, 0xFF, 0xFF, 0xFF, 0x0F]);
  check(-0x80000000, [0xFF, 0xFF, 0xFF, 0xFF, 0x0F]);
});

it('struct float', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeFloatStruct({ x: i })), Buffer.from(o));
    assert.deepEqual(JSON.stringify(schema.decodeFloatStruct(new Uint8Array(o))), JSON.stringify({ x: i }));
  }

  check(0, [0]);
  check(1, [127, 0, 0, 0]);
  check(-1, [127, 1, 0, 0]);
  check(3.1415927410125732, [128, 182, 31, 146]);
  check(-3.1415927410125732, [128, 183, 31, 146]);
  check(Infinity, [255, 0, 0, 0]);
  check(-Infinity, [255, 1, 0, 0]);
  check(NaN, [255, 0, 0, 128]);
});

it('struct string', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeStringStruct({ x: i })), Buffer.from(o));
    assert.deepEqual(schema.decodeStringStruct(new Uint8Array(o)), { x: i });
  }

  check('', [0]);
  check('abc', [97, 98, 99, 0]);
  check('🙉🙈🙊', [240, 159, 153, 137, 240, 159, 153, 136, 240, 159, 153, 138, 0]);
});

it('struct compound', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeCompoundStruct(i)), Buffer.from(o));
    assert.deepEqual(schema.decodeCompoundStruct(new Uint8Array(o)), i);
  }

  check({ x: 0, y: 0 }, [0, 0]);
  check({ x: 1, y: 2 }, [1, 2]);
  check({ x: 12345, y: 54321 }, [185, 96, 177, 168, 3]);
});

it('struct nested', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeNestedStruct(i)), Buffer.from(o));
    assert.deepEqual(schema.decodeNestedStruct(new Uint8Array(o)), i);
  }

  check({ a: 0, b: { x: 0, y: 0 }, c: 0 }, [0, 0, 0, 0]);
  check({ a: 1, b: { x: 2, y: 3 }, c: 4 }, [1, 2, 3, 4]);
  check({ a: 534, b: { x: 12345, y: 54321 }, c: 321 }, [150, 4, 185, 96, 177, 168, 3, 193, 2]);
});

it('message bool', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeBoolMessage(i)), Buffer.from(o));
    assert.deepEqual(schema.decodeBoolMessage(new Uint8Array(o)), i);
  }

  check({}, [0]);
  check({ x: false }, [1, 0, 0]);
  check({ x: true }, [1, 1, 0]);
});

it('message byte', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeByteMessage(i)), Buffer.from(o));
    assert.deepEqual(schema.decodeByteMessage(new Uint8Array(o)), i);
  }

  check({}, [0]);
  check({ x: 234 }, [1, 234, 0]);
});

it('struct byte array', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeByteArrayStruct(i)), Buffer.from(o));
    assert.deepEqual(schema.decodeByteArrayStruct(new Uint8Array(o)), i);
  }

  check({ x: new Uint8Array() }, [0]);
  check({ x: new Uint8Array([4, 5, 6]) }, [3, 4, 5, 6]);
});

it('message uint', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeUintMessage(i)), Buffer.from(o));
    assert.deepEqual(schema.decodeUintMessage(new Uint8Array(o)), i);
  }

  check({}, [0]);
  check({ x: 12345678 }, [1, 206, 194, 241, 5, 0]);
});

it('message int', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeIntMessage(i)), Buffer.from(o));
    assert.deepEqual(schema.decodeIntMessage(new Uint8Array(o)), i);
  }

  check({}, [0]);
  check({ x: 12345678 }, [1, 156, 133, 227, 11, 0]);
});

it('message float', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeFloatMessage(i)), Buffer.from(o));
    assert.deepEqual(schema.decodeFloatMessage(new Uint8Array(o)), i);
  }

  check({}, [0]);
  check({ x: 3.1415927410125732 }, [1, 128, 182, 31, 146, 0]);
});

it('message string', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeStringMessage(i)), Buffer.from(o));
    assert.deepEqual(schema.decodeStringMessage(new Uint8Array(o)), i);
  }

  check({}, [0]);
  check({ x: '' }, [1, 0, 0]);
  check({ x: '🙉🙈🙊' }, [1, 240, 159, 153, 137, 240, 159, 153, 136, 240, 159, 153, 138, 0, 0]);
});

it('message compound', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeCompoundMessage(i)), Buffer.from(o));
    assert.deepEqual(schema.decodeCompoundMessage(new Uint8Array(o)), i);
  }

  check({}, [0]);
  check({ x: 123 }, [1, 123, 0]);
  check({ y: 234 }, [2, 234, 1, 0]);
  check({ x: 123, y: 234 }, [1, 123, 2, 234, 1, 0]);
  check({ y: 234, x: 123 }, [1, 123, 2, 234, 1, 0]);
});

it('message nested', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeNestedMessage(i)), Buffer.from(o));
    assert.deepEqual(schema.decodeNestedMessage(new Uint8Array(o)), i);
  }

  check({}, [0]);
  check({ a: 123, c: 234 }, [1, 123, 3, 234, 1, 0]);
  check({ b: { x: 5, y: 6 } }, [2, 1, 5, 2, 6, 0, 0]);
  check({ b: { x: 5 }, c: 123 }, [2, 1, 5, 0, 3, 123, 0]);
  check({ c: 123, b: { x: 5, y: 6 }, a: 234 }, [1, 234, 1, 2, 1, 5, 2, 6, 0, 3, 123, 0]);
});

it('struct bool array', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeBoolArrayStruct({ x: i })), Buffer.from(o));
    assert.deepEqual(schema.decodeBoolArrayStruct(new Uint8Array(o)), { x: i });
  }

  check([], [0]);
  check([true, false], [2, 1, 0]);
});

it('message bool array', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeBoolArrayMessage(i)), Buffer.from(o));
    assert.deepEqual(schema.decodeBoolArrayMessage(new Uint8Array(o)), i);
  }

  check({}, [0]);
  check({ x: [] }, [1, 0, 0]);
  check({ x: [true, false] }, [1, 2, 1, 0, 0]);
});

it('recursive message', function () {
  function check(i, o) {
    assert.deepEqual(Buffer.from(schema.encodeRecursiveMessage(i)), Buffer.from(o));
    assert.deepEqual(schema.decodeRecursiveMessage(new Uint8Array(o)), i);
  }

  check({}, [0]);
  check({ x: {} }, [1, 0, 0]);
  check({ x: { x: {} } }, [1, 1, 0, 0, 0]);
});

it('binary schema', function () {
  var compiledSchema = kiwi.compileSchema(kiwi.decodeBinarySchema(kiwi.encodeBinarySchema(kiwi.parseSchema(schemaText))));

  function check(message) {
    assert.deepEqual(
      Buffer.from(schema.encodeNestedMessage(message)),
      Buffer.from(compiledSchema.encodeNestedMessage(message)));
  }

  check({ a: 1, c: 4 });
  check({ a: 1, b: {}, c: 4 });
  check({ a: 1, b: { x: 2, y: 3 }, c: 4 });
});

var largeSchemaText = fs.readFileSync(__dirname + '/test-schema-large.kiwi', 'utf8');
var largeSchema = kiwi.compileSchema(kiwi.parseSchema(largeSchemaText));

it('struct with many fields', function () {
  var object = {};
  for (var i = 0; i < 130; i++) object['f' + i] = i;

  var encoded = largeSchema.encodeStruct(object);
  assert.deepEqual(object, largeSchema.decodeStruct(encoded));
});

it('message with many fields', function () {
  var object = {};
  for (var i = 0; i < 130; i++) object['f' + i] = i;

  var encoded = largeSchema.encodeMessage(object);
  assert.deepEqual(object, largeSchema.decodeMessage(encoded));
});

it('message with deprecated fields', function () {
  var nonDeprecated = {
    a: 1,
    b: 2,
    c: [3, 4, 5],
    d: [6, 7, 8],
    e: { x: 123 },
    f: { x: 234 },
    g: 9,
  };

  var deprecated = {
    a: 1,
    c: [3, 4, 5],
    e: { x: 123 },
    g: 9,
  };

  assert.deepEqual(schema.decodeDeprecatedMessage(schema.encodeNonDeprecatedMessage(nonDeprecated)), deprecated);
  assert.deepEqual(schema.decodeNonDeprecatedMessage(schema.encodeDeprecatedMessage(nonDeprecated)), deprecated);
});

it('schema round trip', function () {
  var parsed = kiwi.parseSchema(schemaText);
  var schemaText2 = kiwi.prettyPrintSchema(parsed);
  var parsed2 = kiwi.parseSchema(schemaText2);

  function deleteLocations(ast) {
    ast.definitions.forEach(function (definition) {
      delete definition.line;
      delete definition.column;
      definition.fields.forEach(function (field) {
        delete field.line;
        delete field.column;
      });
    });
  }

  deleteLocations(parsed);
  deleteLocations(parsed2);

  assert.deepEqual(parsed, parsed2);
});
