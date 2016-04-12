var assert = require('assert');
var kiwi = require('../kiwi');
var fs = require('fs');
var schema = kiwi.compileSchema(fs.readFileSync('test-schema.kiwi', 'utf8'));

it('byte', function() {
  function check(i, o) {
    assert.deepEqual(Buffer(schema.encodeByteStruct({x: i})), Buffer(o));
    assert.deepEqual(schema.decodeByteStruct(new Uint8Array(o)), {x: i});
  }

  check(0x00, [0x00]);
  check(0x01, [0x01]);
  check(0x7F, [0x7F]);
  check(0x80, [0x80]);
  check(0xFF, [0xFF]);
});

it('uint', function() {
  function check(i, o) {
    assert.deepEqual(Buffer(schema.encodeUintStruct({x: i})), Buffer(o));
    assert.deepEqual(schema.decodeUintStruct(new Uint8Array(o)), {x: i});
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

it('int', function() {
  function check(i, o) {
    assert.deepEqual(Buffer(schema.encodeIntStruct({x: i})), Buffer(o));
    assert.deepEqual(schema.decodeIntStruct(new Uint8Array(o)), {x: i});
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

it('float', function() {
  function check(i, o) {
    assert.deepEqual(Buffer(schema.encodeFloatStruct({x: i})), Buffer(o));
    assert.deepEqual(schema.decodeFloatStruct(new Uint8Array(o)), {x: i});
  }

  check(0, [0, 0, 0, 0]);
  check(1, [0, 0, 128, 63]);
  check(-1, [0, 0, 128, 191]);
  check(3.1415927410125732, [219, 15, 73, 64]);
  check(-3.1415927410125732, [219, 15, 73, 192]);
});

it('string', function() {
  function check(i, o) {
    assert.deepEqual(Buffer(schema.encodeStringStruct({x: i})), Buffer(o));
    assert.deepEqual(schema.decodeStringStruct(new Uint8Array(o)), {x: i});
  }

  check('', [0]);
  check('abc', [3, 97, 98, 99]);
  check('\0abc\0', [5, 0, 97, 98, 99, 0]);
  check('🙉🙈🙊', [12, 240, 159, 153, 137, 240, 159, 153, 136, 240, 159, 153, 138]);
});
