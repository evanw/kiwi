var assert = require('assert');
var kiwi = require(__dirname + '/../kiwi');
var fs = require('fs');
var schema = kiwi.compileSchema(fs.readFileSync(__dirname + '/test-schema.kiwi', 'utf8'));

it('struct bool', function() {
  function check(i, o) {
    assert.deepEqual(schema.encodeBoolStruct({x: i}), Buffer(o));
    assert.deepEqual(schema.decodeBoolStruct(Buffer(o)), {x: i});
  }

  check(false, [0]);
  check(true, [1]);
});

it('struct byte', function() {
  function check(i, o) {
    assert.deepEqual(schema.encodeByteStruct({x: i}), Buffer(o));
    assert.deepEqual(schema.decodeByteStruct(Buffer(o)), {x: i});
  }

  check(0x00, [0x00]);
  check(0x01, [0x01]);
  check(0x7F, [0x7F]);
  check(0x80, [0x80]);
  check(0xFF, [0xFF]);
});

it('struct uint', function() {
  function check(i, o) {
    assert.deepEqual(schema.encodeUintStruct({x: i}), Buffer(o));
    assert.deepEqual(schema.decodeUintStruct(Buffer(o)), {x: i});
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

it('struct int', function() {
  function check(i, o) {
    assert.deepEqual(schema.encodeIntStruct({x: i}), Buffer(o));
    assert.deepEqual(schema.decodeIntStruct(Buffer(o)), {x: i});
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

it('struct float', function() {
  function check(i, o) {
    assert.deepEqual(schema.encodeFloatStruct({x: i}), Buffer(o));
    assert.deepEqual(schema.decodeFloatStruct(Buffer(o)), {x: i});
  }

  check(0, [0, 0, 0, 0]);
  check(1, [0, 0, 128, 63]);
  check(-1, [0, 0, 128, 191]);
  check(3.1415927410125732, [219, 15, 73, 64]);
  check(-3.1415927410125732, [219, 15, 73, 192]);
});

it('struct string', function() {
  function check(i, o) {
    assert.deepEqual(schema.encodeStringStruct({x: i}), Buffer(o));
    assert.deepEqual(schema.decodeStringStruct(Buffer(o)), {x: i});
  }

  check('', [0]);
  check('abc', [97, 98, 99, 0]);
  check('🙉🙈🙊', [240, 159, 153, 137, 240, 159, 153, 136, 240, 159, 153, 138, 0]);
});

it('struct compound', function() {
  function check(i, o) {
    assert.deepEqual(schema.encodeCompoundStruct(i), Buffer(o));
    assert.deepEqual(schema.decodeCompoundStruct(Buffer(o)), i);
  }

  check({x: 0, y: 0}, [0, 0]);
  check({x: 1, y: 2}, [1, 2]);
  check({x: 12345, y: 54321}, [185, 96, 177, 168, 3]);
});

it('struct nested', function() {
  function check(i, o) {
    assert.deepEqual(schema.encodeNestedStruct(i), Buffer(o));
    assert.deepEqual(schema.decodeNestedStruct(Buffer(o)), i);
  }

  check({a: 0, b: {x: 0, y: 0}, c: 0}, [0, 0, 0, 0]);
  check({a: 1, b: {x: 2, y: 3}, c: 4}, [1, 2, 3, 4]);
  check({a: 534, b: {x: 12345, y: 54321}, c: 321}, [150, 4, 185, 96, 177, 168, 3, 193, 2]);
});

it('message bool', function() {
  function check(i, o) {
    assert.deepEqual(schema.encodeBoolMessage(i), Buffer(o));
    assert.deepEqual(schema.decodeBoolMessage(Buffer(o)), i);
  }

  check({}, [0]);
  check({x: false}, [1, 0, 0]);
  check({x: true}, [1, 1, 0]);
});

it('message byte', function() {
  function check(i, o) {
    assert.deepEqual(schema.encodeByteMessage(i), Buffer(o));
    assert.deepEqual(schema.decodeByteMessage(Buffer(o)), i);
  }

  check({}, [0]);
  check({x: 234}, [1, 234, 0]);
});

it('message uint', function() {
  function check(i, o) {
    assert.deepEqual(schema.encodeUintMessage(i), Buffer(o));
    assert.deepEqual(schema.decodeUintMessage(Buffer(o)), i);
  }

  check({}, [0]);
  check({x: 12345678}, [1, 206, 194, 241, 5, 0]);
});

it('message int', function() {
  function check(i, o) {
    assert.deepEqual(schema.encodeIntMessage(i), Buffer(o));
    assert.deepEqual(schema.decodeIntMessage(Buffer(o)), i);
  }

  check({}, [0]);
  check({x: 12345678}, [1, 156, 133, 227, 11, 0]);
});

it('message float', function() {
  function check(i, o) {
    assert.deepEqual(schema.encodeFloatMessage(i), Buffer(o));
    assert.deepEqual(schema.decodeFloatMessage(Buffer(o)), i);
  }

  check({}, [0]);
  check({x: 3.1415927410125732}, [1, 219, 15, 73, 64, 0]);
});

it('message string', function() {
  function check(i, o) {
    assert.deepEqual(schema.encodeStringMessage(i), Buffer(o));
    assert.deepEqual(schema.decodeStringMessage(Buffer(o)), i);
  }

  check({}, [0]);
  check({x: ''}, [1, 0, 0]);
  check({x: '🙉🙈🙊'}, [1, 240, 159, 153, 137, 240, 159, 153, 136, 240, 159, 153, 138, 0, 0]);
});

it('message compound', function() {
  function check(i, o) {
    assert.deepEqual(schema.encodeCompoundMessage(i), Buffer(o));
    assert.deepEqual(schema.decodeCompoundMessage(Buffer(o)), i);
  }

  check({}, [0]);
  check({x: 123}, [1, 123, 0]);
  check({y: 234}, [2, 234, 1, 0]);
  check({x: 123, y: 234}, [1, 123, 2, 234, 1, 0]);
  check({y: 234, x: 123}, [1, 123, 2, 234, 1, 0]);
});

it('message nested', function() {
  function check(i, o) {
    assert.deepEqual(schema.encodeNestedMessage(i), Buffer(o));
    assert.deepEqual(schema.decodeNestedMessage(Buffer(o)), i);
  }

  check({}, [0]);
  check({a: 123, c: 234}, [1, 123, 3, 234, 1, 0]);
  check({b: {x: 5, y: 6}}, [2, 1, 5, 2, 6, 0, 0]);
  check({b: {x: 5}, c: 123}, [2, 1, 5, 0, 3, 123, 0]);
  check({c: 123, b: {x: 5, y: 6}, a: 234}, [1, 234, 1, 2, 1, 5, 2, 6, 0, 3, 123, 0]);
});

it('struct bool array', function() {
  function check(i, o) {
    assert.deepEqual(schema.encodeBoolArrayStruct({x: i}), Buffer(o));
    assert.deepEqual(schema.decodeBoolArrayStruct(Buffer(o)), {x: i});
  }

  check([], [0]);
  check([true, false], [2, 1, 0]);
});

it('message bool array', function() {
  function check(i, o) {
    assert.deepEqual(schema.encodeBoolArrayMessage(i), Buffer(o));
    assert.deepEqual(schema.decodeBoolArrayMessage(Buffer(o)), i);
  }

  check({}, [0]);
  check({x: []}, [1, 0, 0]);
  check({x: [true, false]}, [1, 2, 1, 0, 0]);
});

it('recursive message', function() {
  function check(i, o) {
    assert.deepEqual(schema.encodeRecursiveMessage(i), Buffer(o));
    assert.deepEqual(schema.decodeRecursiveMessage(Buffer(o)), i);
  }

  check({}, [0]);
  check({x: {}}, [1, 0, 0]);
  check({x: {x: {}}}, [1, 1, 0, 0, 0]);
});

it('required field', function() {
  assert.throws(function() { schema.encodeRequiredField({}); }, Error);
  assert.doesNotThrow(function() { schema.encodeRequiredField({x: 0}); }, Error);

  assert.throws(function() { schema.decodeRequiredField(Buffer([0])); }, Error);
  assert.doesNotThrow(function() { schema.decodeRequiredField(Buffer([1, 1, 0])); }, Error);
});
