var test = exports || test || {}, exports;
test.ByteBuffer = test.ByteBuffer || require("kiwi-schema").ByteBuffer;
test["Enum"] = {
  "100": "A",
  "200": "B",
  "A": 100,
  "B": 200
};

test["decodeEnumStruct"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  result["x"] = this["Enum"][bb.readVarUint()];
  var values = result["y"] = [];
  var length = bb.readVarUint();
  while (length-- > 0) values.push(this["Enum"][bb.readVarUint()]);
  return result;
};

test["encodeEnumStruct"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    var encoded = this["Enum"][value]; if (encoded === void 0) throw new Error("Invalid value " + JSON.stringify(value) + " for enum \"Enum\""); bb.writeVarUint(encoded);
  } else {
    throw new Error("Missing required field \"x\"");
  }

  var value = message["y"];
  if (value != null) {
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      var encoded = this["Enum"][value]; if (encoded === void 0) throw new Error("Invalid value " + JSON.stringify(value) + " for enum \"Enum\""); bb.writeVarUint(encoded);
    }
  } else {
    throw new Error("Missing required field \"y\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeBoolStruct"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  result["x"] = !!bb.readByte();
  return result;
};

test["encodeBoolStruct"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeByte(value);
  } else {
    throw new Error("Missing required field \"x\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeByteStruct"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  result["x"] = bb.readByte();
  return result;
};

test["encodeByteStruct"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeByte(value);
  } else {
    throw new Error("Missing required field \"x\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeIntStruct"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  result["x"] = bb.readVarInt();
  return result;
};

test["encodeIntStruct"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarInt(value);
  } else {
    throw new Error("Missing required field \"x\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeUintStruct"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  result["x"] = bb.readVarUint();
  return result;
};

test["encodeUintStruct"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarUint(value);
  } else {
    throw new Error("Missing required field \"x\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeFloatStruct"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  result["x"] = bb.readVarFloat();
  return result;
};

test["encodeFloatStruct"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarFloat(value);
  } else {
    throw new Error("Missing required field \"x\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeStringStruct"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  result["x"] = bb.readString();
  return result;
};

test["encodeStringStruct"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeString(value);
  } else {
    throw new Error("Missing required field \"x\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeCompoundStruct"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  result["x"] = bb.readVarUint();
  result["y"] = bb.readVarUint();
  return result;
};

test["encodeCompoundStruct"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarUint(value);
  } else {
    throw new Error("Missing required field \"x\"");
  }

  var value = message["y"];
  if (value != null) {
    bb.writeVarUint(value);
  } else {
    throw new Error("Missing required field \"y\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeNestedStruct"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  result["a"] = bb.readVarUint();
  result["b"] = this["decodeCompoundStruct"](bb);
  result["c"] = bb.readVarUint();
  return result;
};

test["encodeNestedStruct"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["a"];
  if (value != null) {
    bb.writeVarUint(value);
  } else {
    throw new Error("Missing required field \"a\"");
  }

  var value = message["b"];
  if (value != null) {
    this["encodeCompoundStruct"](value, bb);
  } else {
    throw new Error("Missing required field \"b\"");
  }

  var value = message["c"];
  if (value != null) {
    bb.writeVarUint(value);
  } else {
    throw new Error("Missing required field \"c\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeBoolMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
    case 0:
      return result;

    case 1:
      result["x"] = !!bb.readByte();
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeBoolMessage"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarUint(1);
    bb.writeByte(value);
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeByteMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
    case 0:
      return result;

    case 1:
      result["x"] = bb.readByte();
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeByteMessage"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarUint(1);
    bb.writeByte(value);
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeIntMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
    case 0:
      return result;

    case 1:
      result["x"] = bb.readVarInt();
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeIntMessage"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarUint(1);
    bb.writeVarInt(value);
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeUintMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
    case 0:
      return result;

    case 1:
      result["x"] = bb.readVarUint();
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeUintMessage"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarUint(1);
    bb.writeVarUint(value);
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeFloatMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
    case 0:
      return result;

    case 1:
      result["x"] = bb.readVarFloat();
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeFloatMessage"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarUint(1);
    bb.writeVarFloat(value);
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeFloat32ArrayMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
    case 0:
      return result;

    case 1:
      var length = bb.readVarUint();
      var values = result["x"] = new Float32Array(length);
      var c = 0;
      while (length-- > 0) { values[c] = (bb.readVarFloat()); c++ }
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeFloat32ArrayMessage"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarUint(1);
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeVarFloat(value);
    }
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeStringMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
    case 0:
      return result;

    case 1:
      result["x"] = bb.readString();
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeStringMessage"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarUint(1);
    bb.writeString(value);
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeCompoundMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
    case 0:
      return result;

    case 1:
      result["x"] = bb.readVarUint();
      break;

    case 2:
      result["y"] = bb.readVarUint();
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeCompoundMessage"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarUint(1);
    bb.writeVarUint(value);
  }

  var value = message["y"];
  if (value != null) {
    bb.writeVarUint(2);
    bb.writeVarUint(value);
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeNestedMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
    case 0:
      return result;

    case 1:
      result["a"] = bb.readVarUint();
      break;

    case 2:
      result["b"] = this["decodeCompoundMessage"](bb);
      break;

    case 3:
      result["c"] = bb.readVarUint();
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeNestedMessage"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["a"];
  if (value != null) {
    bb.writeVarUint(1);
    bb.writeVarUint(value);
  }

  var value = message["b"];
  if (value != null) {
    bb.writeVarUint(2);
    this["encodeCompoundMessage"](value, bb);
  }

  var value = message["c"];
  if (value != null) {
    bb.writeVarUint(3);
    bb.writeVarUint(value);
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeBoolArrayStruct"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  var values = result["x"] = [];
  var length = bb.readVarUint();
  while (length-- > 0) values.push(!!bb.readByte());
  return result;
};

test["encodeBoolArrayStruct"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeByte(value);
    }
  } else {
    throw new Error("Missing required field \"x\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeByteArrayStruct"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  var values = result["x"] = [];
  var length = bb.readVarUint();
  while (length-- > 0) values.push(bb.readByte());
  return result;
};

test["encodeByteArrayStruct"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeByte(value);
    }
  } else {
    throw new Error("Missing required field \"x\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeIntArrayStruct"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  var values = result["x"] = [];
  var length = bb.readVarUint();
  while (length-- > 0) values.push(bb.readVarInt());
  return result;
};

test["encodeIntArrayStruct"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeVarInt(value);
    }
  } else {
    throw new Error("Missing required field \"x\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeUintArrayStruct"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  var values = result["x"] = [];
  var length = bb.readVarUint();
  while (length-- > 0) values.push(bb.readVarUint());
  return result;
};

test["encodeUintArrayStruct"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeVarUint(value);
    }
  } else {
    throw new Error("Missing required field \"x\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeFloatArrayStruct"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  var values = result["x"] = [];
  var length = bb.readVarUint();
  while (length-- > 0) values.push(bb.readVarFloat());
  return result;
};

test["encodeFloatArrayStruct"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeVarFloat(value);
    }
  } else {
    throw new Error("Missing required field \"x\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeStringArrayStruct"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  var values = result["x"] = [];
  var length = bb.readVarUint();
  while (length-- > 0) values.push(bb.readString());
  return result;
};

test["encodeStringArrayStruct"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeString(value);
    }
  } else {
    throw new Error("Missing required field \"x\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeCompoundArrayStruct"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  var values = result["x"] = [];
  var length = bb.readVarUint();
  while (length-- > 0) values.push(bb.readVarUint());
  var values = result["y"] = [];
  var length = bb.readVarUint();
  while (length-- > 0) values.push(bb.readVarUint());
  return result;
};

test["encodeCompoundArrayStruct"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeVarUint(value);
    }
  } else {
    throw new Error("Missing required field \"x\"");
  }

  var value = message["y"];
  if (value != null) {
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeVarUint(value);
    }
  } else {
    throw new Error("Missing required field \"y\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeBoolArrayMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
    case 0:
      return result;

    case 1:
      var values = result["x"] = [];
      var length = bb.readVarUint();
      while (length-- > 0) values.push(!!bb.readByte());
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeBoolArrayMessage"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarUint(1);
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeByte(value);
    }
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeByteArrayMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
    case 0:
      return result;

    case 1:
      var values = result["x"] = [];
      var length = bb.readVarUint();
      while (length-- > 0) values.push(bb.readByte());
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeByteArrayMessage"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarUint(1);
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeByte(value);
    }
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeIntArrayMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
    case 0:
      return result;

    case 1:
      var values = result["x"] = [];
      var length = bb.readVarUint();
      while (length-- > 0) values.push(bb.readVarInt());
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeIntArrayMessage"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarUint(1);
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeVarInt(value);
    }
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeUintArrayMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
    case 0:
      return result;

    case 1:
      var values = result["x"] = [];
      var length = bb.readVarUint();
      while (length-- > 0) values.push(bb.readVarUint());
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeUintArrayMessage"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarUint(1);
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeVarUint(value);
    }
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeFloatArrayMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
    case 0:
      return result;

    case 1:
      var values = result["x"] = [];
      var length = bb.readVarUint();
      while (length-- > 0) values.push(bb.readVarFloat());
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeFloatArrayMessage"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarUint(1);
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeVarFloat(value);
    }
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeStringArrayMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
    case 0:
      return result;

    case 1:
      var values = result["x"] = [];
      var length = bb.readVarUint();
      while (length-- > 0) values.push(bb.readString());
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeStringArrayMessage"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarUint(1);
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeString(value);
    }
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeCompoundArrayMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
    case 0:
      return result;

    case 1:
      var values = result["x"] = [];
      var length = bb.readVarUint();
      while (length-- > 0) values.push(bb.readVarUint());
      break;

    case 2:
      var values = result["y"] = [];
      var length = bb.readVarUint();
      while (length-- > 0) values.push(bb.readVarUint());
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeCompoundArrayMessage"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarUint(1);
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeVarUint(value);
    }
  }

  var value = message["y"];
  if (value != null) {
    bb.writeVarUint(2);
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeVarUint(value);
    }
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeRecursiveMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
    case 0:
      return result;

    case 1:
      result["x"] = this["decodeRecursiveMessage"](bb);
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeRecursiveMessage"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarUint(1);
    this["encodeRecursiveMessage"](value, bb);
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeNonDeprecatedMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
    case 0:
      return result;

    case 1:
      result["a"] = bb.readVarUint();
      break;

    case 2:
      result["b"] = bb.readVarUint();
      break;

    case 3:
      var values = result["c"] = [];
      var length = bb.readVarUint();
      while (length-- > 0) values.push(bb.readVarUint());
      break;

    case 4:
      var values = result["d"] = [];
      var length = bb.readVarUint();
      while (length-- > 0) values.push(bb.readVarUint());
      break;

    case 5:
      result["e"] = this["decodeByteStruct"](bb);
      break;

    case 6:
      result["f"] = this["decodeByteStruct"](bb);
      break;

    case 7:
      result["g"] = bb.readVarUint();
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeNonDeprecatedMessage"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["a"];
  if (value != null) {
    bb.writeVarUint(1);
    bb.writeVarUint(value);
  }

  var value = message["b"];
  if (value != null) {
    bb.writeVarUint(2);
    bb.writeVarUint(value);
  }

  var value = message["c"];
  if (value != null) {
    bb.writeVarUint(3);
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeVarUint(value);
    }
  }

  var value = message["d"];
  if (value != null) {
    bb.writeVarUint(4);
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeVarUint(value);
    }
  }

  var value = message["e"];
  if (value != null) {
    bb.writeVarUint(5);
    this["encodeByteStruct"](value, bb);
  }

  var value = message["f"];
  if (value != null) {
    bb.writeVarUint(6);
    this["encodeByteStruct"](value, bb);
  }

  var value = message["g"];
  if (value != null) {
    bb.writeVarUint(7);
    bb.writeVarUint(value);
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeDeprecatedMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
    case 0:
      return result;

    case 1:
      result["a"] = bb.readVarUint();
      break;

    case 2:
      bb.readVarUint();
      break;

    case 3:
      var values = result["c"] = [];
      var length = bb.readVarUint();
      while (length-- > 0) values.push(bb.readVarUint());
      break;

    case 4:
      var length = bb.readVarUint();
      while (length-- > 0) bb.readVarUint();
      break;

    case 5:
      result["e"] = this["decodeByteStruct"](bb);
      break;

    case 6:
      this["decodeByteStruct"](bb);
      break;

    case 7:
      result["g"] = bb.readVarUint();
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeDeprecatedMessage"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["a"];
  if (value != null) {
    bb.writeVarUint(1);
    bb.writeVarUint(value);
  }

  var value = message["c"];
  if (value != null) {
    bb.writeVarUint(3);
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeVarUint(value);
    }
  }

  var value = message["e"];
  if (value != null) {
    bb.writeVarUint(5);
    this["encodeByteStruct"](value, bb);
  }

  var value = message["g"];
  if (value != null) {
    bb.writeVarUint(7);
    bb.writeVarUint(value);
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

test["decodeSortedStruct"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  result["a1"] = !!bb.readByte();
  result["b1"] = bb.readByte();
  result["c1"] = bb.readVarInt();
  result["d1"] = bb.readVarUint();
  result["e1"] = bb.readVarFloat();
  result["f1"] = bb.readString();
  result["a2"] = !!bb.readByte();
  result["b2"] = bb.readByte();
  result["c2"] = bb.readVarInt();
  result["d2"] = bb.readVarUint();
  result["e2"] = bb.readVarFloat();
  result["f2"] = bb.readString();
  var values = result["a3"] = [];
  var length = bb.readVarUint();
  while (length-- > 0) values.push(!!bb.readByte());
  var values = result["b3"] = [];
  var length = bb.readVarUint();
  while (length-- > 0) values.push(bb.readByte());
  var values = result["c3"] = [];
  var length = bb.readVarUint();
  while (length-- > 0) values.push(bb.readVarInt());
  var values = result["d3"] = [];
  var length = bb.readVarUint();
  while (length-- > 0) values.push(bb.readVarUint());
  var values = result["e3"] = [];
  var length = bb.readVarUint();
  while (length-- > 0) values.push(bb.readVarFloat());
  var values = result["f3"] = [];
  var length = bb.readVarUint();
  while (length-- > 0) values.push(bb.readString());
  return result;
};

test["encodeSortedStruct"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["a1"];
  if (value != null) {
    bb.writeByte(value);
  } else {
    throw new Error("Missing required field \"a1\"");
  }

  var value = message["b1"];
  if (value != null) {
    bb.writeByte(value);
  } else {
    throw new Error("Missing required field \"b1\"");
  }

  var value = message["c1"];
  if (value != null) {
    bb.writeVarInt(value);
  } else {
    throw new Error("Missing required field \"c1\"");
  }

  var value = message["d1"];
  if (value != null) {
    bb.writeVarUint(value);
  } else {
    throw new Error("Missing required field \"d1\"");
  }

  var value = message["e1"];
  if (value != null) {
    bb.writeVarFloat(value);
  } else {
    throw new Error("Missing required field \"e1\"");
  }

  var value = message["f1"];
  if (value != null) {
    bb.writeString(value);
  } else {
    throw new Error("Missing required field \"f1\"");
  }

  var value = message["a2"];
  if (value != null) {
    bb.writeByte(value);
  } else {
    throw new Error("Missing required field \"a2\"");
  }

  var value = message["b2"];
  if (value != null) {
    bb.writeByte(value);
  } else {
    throw new Error("Missing required field \"b2\"");
  }

  var value = message["c2"];
  if (value != null) {
    bb.writeVarInt(value);
  } else {
    throw new Error("Missing required field \"c2\"");
  }

  var value = message["d2"];
  if (value != null) {
    bb.writeVarUint(value);
  } else {
    throw new Error("Missing required field \"d2\"");
  }

  var value = message["e2"];
  if (value != null) {
    bb.writeVarFloat(value);
  } else {
    throw new Error("Missing required field \"e2\"");
  }

  var value = message["f2"];
  if (value != null) {
    bb.writeString(value);
  } else {
    throw new Error("Missing required field \"f2\"");
  }

  var value = message["a3"];
  if (value != null) {
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeByte(value);
    }
  } else {
    throw new Error("Missing required field \"a3\"");
  }

  var value = message["b3"];
  if (value != null) {
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeByte(value);
    }
  } else {
    throw new Error("Missing required field \"b3\"");
  }

  var value = message["c3"];
  if (value != null) {
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeVarInt(value);
    }
  } else {
    throw new Error("Missing required field \"c3\"");
  }

  var value = message["d3"];
  if (value != null) {
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeVarUint(value);
    }
  } else {
    throw new Error("Missing required field \"d3\"");
  }

  var value = message["e3"];
  if (value != null) {
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeVarFloat(value);
    }
  } else {
    throw new Error("Missing required field \"e3\"");
  }

  var value = message["f3"];
  if (value != null) {
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      bb.writeString(value);
    }
  } else {
    throw new Error("Missing required field \"f3\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};
