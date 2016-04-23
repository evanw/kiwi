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
    switch (bb.readByte()) {
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
    switch (bb.readByte()) {
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
    switch (bb.readByte()) {
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
    switch (bb.readByte()) {
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
    switch (bb.readByte()) {
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

test["decodeStringMessage"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readByte()) {
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
    switch (bb.readByte()) {
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
    switch (bb.readByte()) {
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
    switch (bb.readByte()) {
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
    switch (bb.readByte()) {
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
    switch (bb.readByte()) {
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
    switch (bb.readByte()) {
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
    switch (bb.readByte()) {
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
    switch (bb.readByte()) {
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
    switch (bb.readByte()) {
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
    switch (bb.readByte()) {
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

test["decodeRequiredField"] = function(bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readByte()) {
    case 0:
      if (result["x"] == null) throw new Error("Missing required field \"x\"");
      return result;

    case 1:
      result["x"] = bb.readVarInt();
      break;

    default:
      throw new Error("Attempted to parse invalid message");
    }
  }
};

test["encodeRequiredField"] = function(message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarUint(1);
    bb.writeVarInt(value);
  } else {
    throw new Error("Missing required field \"x\"");
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};
