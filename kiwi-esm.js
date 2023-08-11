// bb.ts
var int32 = new Int32Array(1);
var float32 = new Float32Array(int32.buffer);
var ByteBuffer = class {
  constructor(data) {
    if (data && !(data instanceof Uint8Array)) {
      throw new Error("Must initialize a ByteBuffer with a Uint8Array");
    }
    this._data = data || new Uint8Array(256);
    this._index = 0;
    this.length = data ? data.length : 0;
  }
  toUint8Array() {
    return this._data.subarray(0, this.length);
  }
  readByte() {
    if (this._index + 1 > this._data.length) {
      throw new Error("Index out of bounds");
    }
    return this._data[this._index++];
  }
  readByteArray() {
    let length = this.readVarUint();
    let start = this._index;
    let end = start + length;
    if (end > this._data.length) {
      throw new Error("Read array out of bounds");
    }
    this._index = end;
    let result = new Uint8Array(length);
    result.set(this._data.subarray(start, end));
    return result;
  }
  readVarFloat() {
    let index = this._index;
    let data = this._data;
    let length = data.length;
    if (index + 1 > length) {
      throw new Error("Index out of bounds");
    }
    let first = data[index];
    if (first === 0) {
      this._index = index + 1;
      return 0;
    }
    if (index + 4 > length) {
      throw new Error("Index out of bounds");
    }
    let bits = first | data[index + 1] << 8 | data[index + 2] << 16 | data[index + 3] << 24;
    this._index = index + 4;
    bits = bits << 23 | bits >>> 9;
    int32[0] = bits;
    return float32[0];
  }
  readVarUint() {
    let value = 0;
    let shift = 0;
    do {
      var byte = this.readByte();
      value |= (byte & 127) << shift;
      shift += 7;
    } while (byte & 128 && shift < 35);
    return value >>> 0;
  }
  readVarInt() {
    let value = this.readVarUint() | 0;
    return value & 1 ? ~(value >>> 1) : value >>> 1;
  }
  readVarUint64() {
    let value = BigInt(0);
    let shift = BigInt(0);
    let seven = BigInt(7);
    let byte;
    while ((byte = this.readByte()) & 128 && shift < 56) {
      value |= BigInt(byte & 127) << shift;
      shift += seven;
    }
    value |= BigInt(byte) << shift;
    return value;
  }
  readVarInt64() {
    let value = this.readVarUint64();
    let one = BigInt(1);
    let sign = value & one;
    value >>= one;
    return sign ? ~value : value;
  }
  readString() {
    let result = "";
    while (true) {
      let codePoint;
      let a = this.readByte();
      if (a < 192) {
        codePoint = a;
      } else {
        let b = this.readByte();
        if (a < 224) {
          codePoint = (a & 31) << 6 | b & 63;
        } else {
          let c = this.readByte();
          if (a < 240) {
            codePoint = (a & 15) << 12 | (b & 63) << 6 | c & 63;
          } else {
            let d = this.readByte();
            codePoint = (a & 7) << 18 | (b & 63) << 12 | (c & 63) << 6 | d & 63;
          }
        }
      }
      if (codePoint === 0) {
        break;
      }
      if (codePoint < 65536) {
        result += String.fromCharCode(codePoint);
      } else {
        codePoint -= 65536;
        result += String.fromCharCode((codePoint >> 10) + 55296, (codePoint & (1 << 10) - 1) + 56320);
      }
    }
    return result;
  }
  _growBy(amount) {
    if (this.length + amount > this._data.length) {
      let data = new Uint8Array(this.length + amount << 1);
      data.set(this._data);
      this._data = data;
    }
    this.length += amount;
  }
  writeByte(value) {
    let index = this.length;
    this._growBy(1);
    this._data[index] = value;
  }
  writeByteArray(value) {
    this.writeVarUint(value.length);
    let index = this.length;
    this._growBy(value.length);
    this._data.set(value, index);
  }
  writeVarFloat(value) {
    let index = this.length;
    float32[0] = value;
    let bits = int32[0];
    bits = bits >>> 23 | bits << 9;
    if ((bits & 255) === 0) {
      this.writeByte(0);
      return;
    }
    this._growBy(4);
    let data = this._data;
    data[index] = bits;
    data[index + 1] = bits >> 8;
    data[index + 2] = bits >> 16;
    data[index + 3] = bits >> 24;
  }
  writeVarUint(value) {
    if (value < 0 || value > 4294967295)
      throw new Error("Outside uint range: " + value);
    do {
      let byte = value & 127;
      value >>>= 7;
      this.writeByte(value ? byte | 128 : byte);
    } while (value);
  }
  writeVarInt(value) {
    if (value < -2147483648 || value > 2147483647)
      throw new Error("Outside int range: " + value);
    this.writeVarUint((value << 1 ^ value >> 31) >>> 0);
  }
  writeVarUint64(value) {
    if (typeof value === "string")
      value = BigInt(value);
    else if (typeof value !== "bigint")
      throw new Error("Expected bigint but got " + typeof value + ": " + value);
    if (value < 0 || value > BigInt("0xFFFFFFFFFFFFFFFF"))
      throw new Error("Outside uint64 range: " + value);
    let mask = BigInt(127);
    let seven = BigInt(7);
    for (let i = 0; value > mask && i < 8; i++) {
      this.writeByte(Number(value & mask) | 128);
      value >>= seven;
    }
    this.writeByte(Number(value));
  }
  writeVarInt64(value) {
    if (typeof value === "string")
      value = BigInt(value);
    else if (typeof value !== "bigint")
      throw new Error("Expected bigint but got " + typeof value + ": " + value);
    if (value < -BigInt("0x8000000000000000") || value > BigInt("0x7FFFFFFFFFFFFFFF"))
      throw new Error("Outside int64 range: " + value);
    let one = BigInt(1);
    this.writeVarUint64(value < 0 ? ~(value << one) : value << one);
  }
  writeString(value) {
    let codePoint;
    for (let i = 0; i < value.length; i++) {
      let a = value.charCodeAt(i);
      if (i + 1 === value.length || a < 55296 || a >= 56320) {
        codePoint = a;
      } else {
        let b = value.charCodeAt(++i);
        codePoint = (a << 10) + b + (65536 - (55296 << 10) - 56320);
      }
      if (codePoint === 0) {
        throw new Error("Cannot encode a string containing the null character");
      }
      if (codePoint < 128) {
        this.writeByte(codePoint);
      } else {
        if (codePoint < 2048) {
          this.writeByte(codePoint >> 6 & 31 | 192);
        } else {
          if (codePoint < 65536) {
            this.writeByte(codePoint >> 12 & 15 | 224);
          } else {
            this.writeByte(codePoint >> 18 & 7 | 240);
            this.writeByte(codePoint >> 12 & 63 | 128);
          }
          this.writeByte(codePoint >> 6 & 63 | 128);
        }
        this.writeByte(codePoint & 63 | 128);
      }
    }
    this.writeByte(0);
  }
};

// util.ts
function quote(text) {
  return JSON.stringify(text);
}
function error(text, line, column) {
  var error2 = new Error(text);
  error2.line = line;
  error2.column = column;
  throw error2;
}

// js.ts
function compileDecode(definition, definitions) {
  let lines = [];
  let indent = "  ";
  lines.push("function (bb) {");
  lines.push("  var result = {};");
  lines.push("  if (!(bb instanceof this.ByteBuffer)) {");
  lines.push("    bb = new this.ByteBuffer(bb);");
  lines.push("  }");
  lines.push("");
  if (definition.kind === "MESSAGE") {
    lines.push("  while (true) {");
    lines.push("    switch (bb.readVarUint()) {");
    lines.push("      case 0:");
    lines.push("        return result;");
    lines.push("");
    indent = "        ";
  }
  for (let i = 0; i < definition.fields.length; i++) {
    let field = definition.fields[i];
    let code;
    switch (field.type) {
      case "bool": {
        code = "!!bb.readByte()";
        break;
      }
      case "byte": {
        code = "bb.readByte()";
        break;
      }
      case "int": {
        code = "bb.readVarInt()";
        break;
      }
      case "uint": {
        code = "bb.readVarUint()";
        break;
      }
      case "float": {
        code = "bb.readVarFloat()";
        break;
      }
      case "string": {
        code = "bb.readString()";
        break;
      }
      case "int64": {
        code = "bb.readVarInt64()";
        break;
      }
      case "uint64": {
        code = "bb.readVarUint64()";
        break;
      }
      default: {
        let type = definitions[field.type];
        if (!type) {
          error("Invalid type " + quote(field.type) + " for field " + quote(field.name), field.line, field.column);
        } else if (type.kind === "ENUM") {
          code = "this[" + quote(type.name) + "][bb.readVarUint()]";
        } else {
          code = "this[" + quote("decode" + type.name) + "](bb)";
        }
      }
    }
    if (definition.kind === "MESSAGE") {
      lines.push("      case " + field.value + ":");
    }
    if (field.isArray) {
      if (field.isDeprecated) {
        if (field.type === "byte") {
          lines.push(indent + "bb.readByteArray();");
        } else {
          lines.push(indent + "var length = bb.readVarUint();");
          lines.push(indent + "while (length-- > 0) " + code + ";");
        }
      } else {
        if (field.type === "byte") {
          lines.push(indent + "result[" + quote(field.name) + "] = bb.readByteArray();");
        } else {
          lines.push(indent + "var length = bb.readVarUint();");
          lines.push(indent + "var values = result[" + quote(field.name) + "] = Array(length);");
          lines.push(indent + "for (var i = 0; i < length; i++) values[i] = " + code + ";");
        }
      }
    } else {
      if (field.isDeprecated) {
        lines.push(indent + code + ";");
      } else {
        lines.push(indent + "result[" + quote(field.name) + "] = " + code + ";");
      }
    }
    if (definition.kind === "MESSAGE") {
      lines.push("        break;");
      lines.push("");
    }
  }
  if (definition.kind === "MESSAGE") {
    lines.push("      default:");
    lines.push('        throw new Error("Attempted to parse invalid message");');
    lines.push("    }");
    lines.push("  }");
  } else {
    lines.push("  return result;");
  }
  lines.push("}");
  return lines.join("\n");
}
function compileEncode(definition, definitions) {
  let lines = [];
  lines.push("function (message, bb) {");
  lines.push("  var isTopLevel = !bb;");
  lines.push("  if (isTopLevel) bb = new this.ByteBuffer();");
  for (let j = 0; j < definition.fields.length; j++) {
    let field = definition.fields[j];
    let code;
    if (field.isDeprecated) {
      continue;
    }
    switch (field.type) {
      case "bool": {
        code = "bb.writeByte(value);";
        break;
      }
      case "byte": {
        code = "bb.writeByte(value);";
        break;
      }
      case "int": {
        code = "bb.writeVarInt(value);";
        break;
      }
      case "uint": {
        code = "bb.writeVarUint(value);";
        break;
      }
      case "float": {
        code = "bb.writeVarFloat(value);";
        break;
      }
      case "string": {
        code = "bb.writeString(value);";
        break;
      }
      case "int64": {
        code = "bb.writeVarInt64(value);";
        break;
      }
      case "uint64": {
        code = "bb.writeVarUint64(value);";
        break;
      }
      default: {
        let type = definitions[field.type];
        if (!type) {
          throw new Error("Invalid type " + quote(field.type) + " for field " + quote(field.name));
        } else if (type.kind === "ENUM") {
          code = "var encoded = this[" + quote(type.name) + '][value]; if (encoded === void 0) throw new Error("Invalid value " + JSON.stringify(value) + ' + quote(" for enum " + quote(type.name)) + "); bb.writeVarUint(encoded);";
        } else {
          code = "this[" + quote("encode" + type.name) + "](value, bb);";
        }
      }
    }
    lines.push("");
    lines.push("  var value = message[" + quote(field.name) + "];");
    lines.push("  if (value != null) {");
    if (definition.kind === "MESSAGE") {
      lines.push("    bb.writeVarUint(" + field.value + ");");
    }
    if (field.isArray) {
      if (field.type === "byte") {
        lines.push("    bb.writeByteArray(value);");
      } else {
        lines.push("    var values = value, n = values.length;");
        lines.push("    bb.writeVarUint(n);");
        lines.push("    for (var i = 0; i < n; i++) {");
        lines.push("      value = values[i];");
        lines.push("      " + code);
        lines.push("    }");
      }
    } else {
      lines.push("    " + code);
    }
    if (definition.kind === "STRUCT") {
      lines.push("  } else {");
      lines.push("    throw new Error(" + quote("Missing required field " + quote(field.name)) + ");");
    }
    lines.push("  }");
  }
  if (definition.kind === "MESSAGE") {
    lines.push("  bb.writeVarUint(0);");
  }
  lines.push("");
  lines.push("  if (isTopLevel) return bb.toUint8Array();");
  lines.push("}");
  return lines.join("\n");
}
function compileSchemaJS(schema) {
  let definitions = {};
  let name = schema.package;
  let js = [];
  if (name !== null) {
    js.push("var " + name + " = exports || " + name + " || {}, exports;");
  } else {
    js.push("var exports = exports || {};");
    name = "exports";
  }
  js.push(name + ".ByteBuffer = " + name + '.ByteBuffer || require("kiwi-schema").ByteBuffer;');
  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    definitions[definition.name] = definition;
  }
  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    switch (definition.kind) {
      case "ENUM": {
        let value = {};
        for (let j = 0; j < definition.fields.length; j++) {
          let field = definition.fields[j];
          value[field.name] = field.value;
          value[field.value] = field.name;
        }
        js.push(name + "[" + quote(definition.name) + "] = " + JSON.stringify(value, null, 2) + ";");
        break;
      }
      case "STRUCT":
      case "MESSAGE": {
        js.push("");
        js.push(name + "[" + quote("decode" + definition.name) + "] = " + compileDecode(definition, definitions) + ";");
        js.push("");
        js.push(name + "[" + quote("encode" + definition.name) + "] = " + compileEncode(definition, definitions) + ";");
        break;
      }
      default: {
        error("Invalid definition kind " + quote(definition.kind), definition.line, definition.column);
        break;
      }
    }
  }
  js.push("");
  return js.join("\n");
}
function compileSchema(schema) {
  let result = {
    ByteBuffer
  };
  new Function("exports", compileSchemaJS(schema))(result);
  return result;
}

// cpp-callback.ts
function argumentForField(definitions, type, name) {
  switch (type) {
    case "bool":
      return { type: "bool ", name };
    case "byte":
      return { type: "uint8_t ", name };
    case "int":
      return { type: "int32_t ", name };
    case "uint":
      return { type: "uint32_t ", name };
    case "float":
      return { type: "float ", name };
    case "string":
      return { type: "const char *", name };
    case "int64":
      return { type: "int64_t ", name };
    case "uint64":
      return { type: "uint64_t ", name };
    default: {
      let definition = definitions[type];
      if (definition.kind === "ENUM")
        return { type: definition.name + " ", name };
      return null;
    }
  }
}
function extractStructArguments(definitions, prefix, fields, allowArrays) {
  let args = [];
  for (let i = 0; i < fields.length; i++) {
    let field = fields[i];
    let name = prefix + field.name;
    if (field.isArray && !allowArrays) {
      return null;
    }
    let arg = argumentForField(definitions, field.type, name);
    if (arg !== null) {
      args.push(arg);
      continue;
    }
    let type = definitions[field.type];
    if (type.kind !== "STRUCT") {
      return null;
    }
    let typeArgs = extractStructArguments(definitions, name + "_", type.fields, false);
    if (typeArgs === null) {
      return null;
    }
    args.push.apply(args, typeArgs);
  }
  return args;
}
function argToDeclaration(arg) {
  return arg.type + arg.name;
}
function argToName(arg) {
  return arg.name;
}
function argToNotRead(arg) {
  switch (arg.type) {
    case "bool ":
    case "uint8_t ":
      return "!bb.readByte(" + arg.name + ")";
    case "int32_t ":
      return "!bb.readVarInt(" + arg.name + ")";
    case "uint32_t ":
      return "!bb.readVarUint(" + arg.name + ")";
    case "float ":
      return "!bb.readVarFloat(" + arg.name + ")";
    case "const char *":
      return "!bb.readString(" + arg.name + ")";
    case "int64_t ":
      return "!bb.readVarInt64(" + arg.name + ")";
    case "uint64_t ":
      return "!bb.readVarUint64(" + arg.name + ")";
    default:
      return "!bb.readVarUint(reinterpret_cast<uint32_t &>(" + arg.name + "))";
  }
}
function argToWrite(arg) {
  switch (arg.type) {
    case "bool ":
    case "uint8_t ":
      return "_bb.writeByte(" + arg.name + ")";
    case "int32_t ":
      return "_bb.writeVarInt(" + arg.name + ")";
    case "uint32_t ":
      return "_bb.writeVarUint(" + arg.name + ")";
    case "float ":
      return "_bb.writeVarFloat(" + arg.name + ")";
    case "const char *":
      return "_bb.writeString(" + arg.name + ")";
    case "int64_t ":
      return "_bb.writeVarInt64(" + arg.name + ")";
    case "uint64_t ":
      return "_bb.writeVarUint64(" + arg.name + ")";
    default:
      return "_bb.writeVarUint(static_cast<uint32_t>(" + arg.name + "))";
  }
}
function emitReadField(cpp, definitions, definition, field, indent) {
  let name = field.name;
  if (field.isArray) {
    let count = "_" + name + "_count";
    cpp.push(indent + "uint32_t " + count + ";");
    cpp.push(indent + "if (!bb.readVarUint(" + count + ")) return false;");
    if (!field.isDeprecated) {
      cpp.push(indent + "visitor.visit" + definition.name + "_" + field.name + "_count(" + count + ");");
    }
    cpp.push(indent + "while (" + count + "-- > 0) {");
    indent += "  ";
    name += "_element";
  }
  let args = extractStructArguments(definitions, "", [field], true);
  if (args !== null) {
    for (let i = 0; i < args.length; i++) {
      cpp.push(indent + argToDeclaration(args[i]) + ";");
    }
    cpp.push(indent + "if (" + args.map(argToNotRead).join(" || ") + ") return false;");
    if (!field.isDeprecated) {
      cpp.push(indent + "visitor.visit" + definition.name + "_" + name + "(" + args.map(argToName).join(", ") + ");");
    }
  } else {
    if (!field.isDeprecated) {
      cpp.push(indent + "visitor.visit" + definition.name + "_" + name + "();");
    }
    let type = definitions[field.type];
    cpp.push(indent + "if (!parse" + type.name + "(bb, visitor)) return false;");
  }
  if (field.isArray) {
    cpp.push(indent.slice(2) + "}");
  }
}
function compileSchemaCallbackCPP(schema) {
  let definitions = {};
  let cpp = [];
  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    definitions[definition.name] = definition;
  }
  if (schema.package !== null) {
    cpp.push("#ifndef INCLUDE_" + schema.package.toUpperCase() + "_H");
    cpp.push("#define INCLUDE_" + schema.package.toUpperCase() + "_H");
    cpp.push("");
  }
  cpp.push('#include "kiwi.h"');
  cpp.push("");
  if (schema.package !== null) {
    cpp.push("namespace " + schema.package + " {");
    cpp.push("");
  }
  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    if (definition.kind === "ENUM") {
      cpp.push("enum class " + definition.name + " : uint32_t {");
      for (let j = 0; j < definition.fields.length; j++) {
        let field = definition.fields[j];
        if (!field.isDeprecated) {
          cpp.push("  " + field.name + " = " + field.value + ",");
        }
      }
      cpp.push("};");
      cpp.push("");
    }
  }
  for (let pass = 0; pass < 2; pass++) {
    let suffix = ") = 0;";
    if (pass === 0) {
      cpp.push("class Visitor {");
      cpp.push("public:");
    } else {
      cpp.push("class Writer : public Visitor {");
      cpp.push("private:");
      cpp.push("  kiwi::ByteBuffer &_bb;");
      cpp.push("public:");
      cpp.push("  Writer(kiwi::ByteBuffer &bb) : _bb(bb) {}");
      suffix = ") override;";
    }
    for (let i = 0; i < schema.definitions.length; i++) {
      let definition = schema.definitions[i];
      if (definition.kind === "STRUCT") {
        let args = extractStructArguments(definitions, "", definition.fields, false);
        if (args !== null) {
          cpp.push("  virtual void visit" + definition.name + "(" + args.map(argToDeclaration).join(", ") + suffix);
          continue;
        }
      }
      if (definition.kind === "STRUCT" || definition.kind === "MESSAGE") {
        cpp.push("  virtual void begin" + definition.name + "(" + suffix);
        for (let j = 0; j < definition.fields.length; j++) {
          let field = definition.fields[j];
          if (field.isDeprecated) {
            continue;
          }
          let name = field.name;
          if (field.isArray) {
            cpp.push("  virtual void visit" + definition.name + "_" + field.name + "_count(uint32_t size" + suffix);
            name += "_element";
          }
          let args = extractStructArguments(definitions, "", [field], true);
          if (args !== null) {
            cpp.push("  virtual void visit" + definition.name + "_" + name + "(" + args.map(argToDeclaration).join(", ") + suffix);
          } else {
            cpp.push("  virtual void visit" + definition.name + "_" + name + "(" + suffix);
          }
        }
        cpp.push("  virtual void end" + definition.name + "(" + suffix);
      }
    }
    cpp.push("};");
    cpp.push("");
  }
  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    if (definition.kind === "STRUCT" || definition.kind === "MESSAGE") {
      cpp.push("bool parse" + definition.name + "(kiwi::ByteBuffer &bb, Visitor &visitor);");
    }
  }
  cpp.push("");
  cpp.push("#ifdef IMPLEMENT_SCHEMA_H");
  cpp.push("");
  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    if (definition.kind === "STRUCT") {
      let args = extractStructArguments(definitions, "", definition.fields, false);
      if (args !== null) {
        cpp.push("bool parse" + definition.name + "(kiwi::ByteBuffer &bb, Visitor &visitor) {");
        for (let j = 0; j < args.length; j++) {
          cpp.push("  " + argToDeclaration(args[j]) + ";");
        }
        cpp.push("  if (" + args.map(argToNotRead).join(" || ") + ") return false;");
        cpp.push("  visitor.visit" + definition.name + "(" + args.map(argToName).join(", ") + ");");
        cpp.push("  return true;");
        cpp.push("}");
        cpp.push("");
      } else {
        cpp.push("bool parse" + definition.name + "(kiwi::ByteBuffer &bb, Visitor &visitor) {");
        cpp.push("  visitor.begin" + definition.name + "();");
        for (let j = 0; j < definition.fields.length; j++) {
          let field = definition.fields[j];
          emitReadField(cpp, definitions, definition, field, "  ");
        }
        cpp.push("  visitor.end" + definition.name + "();");
        cpp.push("  return true;");
        cpp.push("}");
        cpp.push("");
      }
    } else if (definition.kind === "MESSAGE") {
      cpp.push("bool parse" + definition.name + "(kiwi::ByteBuffer &bb, Visitor &visitor) {");
      cpp.push("  visitor.begin" + definition.name + "();");
      cpp.push("  while (true) {");
      cpp.push("    uint32_t _type;");
      cpp.push("    if (!bb.readVarUint(_type)) return false;");
      cpp.push("    switch (_type) {");
      cpp.push("      case 0: {");
      cpp.push("        visitor.end" + definition.name + "();");
      cpp.push("        return true;");
      cpp.push("      }");
      for (let j = 0; j < definition.fields.length; j++) {
        let field = definition.fields[j];
        cpp.push("      case " + field.value + ": {");
        emitReadField(cpp, definitions, definition, field, "        ");
        cpp.push("        break;");
        cpp.push("      }");
      }
      cpp.push("      default: return false;");
      cpp.push("    }");
      cpp.push("  }");
      cpp.push("}");
      cpp.push("");
    }
  }
  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    if (definition.kind === "STRUCT") {
      let args = extractStructArguments(definitions, "", definition.fields, false);
      if (args !== null) {
        cpp.push("void Writer::visit" + definition.name + "(" + args.map(argToDeclaration).join(", ") + ") {");
        for (let j = 0; j < args.length; j++) {
          cpp.push("  " + argToWrite(args[j]) + ";");
        }
        cpp.push("}");
        cpp.push("");
        continue;
      }
    }
    if (definition.kind === "STRUCT" || definition.kind === "MESSAGE") {
      cpp.push("void Writer::begin" + definition.name + "() {");
      cpp.push("}");
      cpp.push("");
      for (let j = 0; j < definition.fields.length; j++) {
        let field = definition.fields[j];
        if (field.isDeprecated) {
          continue;
        }
        let name = field.name;
        if (field.isArray) {
          cpp.push("void Writer::visit" + definition.name + "_" + field.name + "_count(uint32_t size) {");
          if (definition.kind === "MESSAGE") {
            cpp.push("  _bb.writeVarUint(" + field.value + ");");
          }
          cpp.push("  _bb.writeVarUint(size);");
          cpp.push("}");
          cpp.push("");
          name += "_element";
        }
        let args = extractStructArguments(definitions, "", [field], true);
        if (args !== null) {
          cpp.push("void Writer::visit" + definition.name + "_" + name + "(" + args.map(argToDeclaration).join(", ") + ") {");
          if (definition.kind === "MESSAGE" && !field.isArray) {
            cpp.push("  _bb.writeVarUint(" + field.value + ");");
          }
          for (let k = 0; k < args.length; k++) {
            cpp.push("  " + argToWrite(args[k]) + ";");
          }
          cpp.push("}");
          cpp.push("");
        } else {
          cpp.push("void Writer::visit" + definition.name + "_" + name + "() {");
          if (definition.kind === "MESSAGE" && !field.isArray) {
            cpp.push("  _bb.writeVarUint(" + field.value + ");");
          }
          cpp.push("}");
          cpp.push("");
        }
      }
      cpp.push("void Writer::end" + definition.name + "() {");
      if (definition.kind === "MESSAGE") {
        cpp.push("  _bb.writeVarUint(0);");
      }
      cpp.push("}");
      cpp.push("");
    }
  }
  cpp.push("#endif");
  cpp.push("");
  if (schema.package !== null) {
    cpp.push("}");
    cpp.push("");
    cpp.push("#endif");
    cpp.push("");
  }
  return cpp.join("\n");
}

// cpp.ts
function cppType(definitions, field, isArray) {
  let type;
  switch (field.type) {
    case "bool":
      type = "bool";
      break;
    case "byte":
      type = "uint8_t";
      break;
    case "int":
      type = "int32_t";
      break;
    case "uint":
      type = "uint32_t";
      break;
    case "float":
      type = "float";
      break;
    case "string":
      type = "kiwi::String";
      break;
    case "int64":
      type = "int64_t";
      break;
    case "uint64":
      type = "uint64_t";
      break;
    default: {
      let definition = definitions[field.type];
      if (!definition) {
        error("Invalid type " + quote(field.type) + " for field " + quote(field.name), field.line, field.column);
      }
      type = definition.name;
      break;
    }
  }
  if (isArray) {
    type = "kiwi::Array<" + type + ">";
  }
  return type;
}
function cppFieldName(field) {
  return "_data_" + field.name;
}
function cppFlagIndex(i) {
  return i >> 5;
}
function cppFlagMask(i) {
  return 1 << i % 32 >>> 0;
}
function cppIsFieldPointer(definitions, field) {
  return !field.isArray && field.type in definitions && definitions[field.type].kind !== "ENUM";
}
function compileSchemaCPP(schema) {
  let definitions = {};
  let cpp = [];
  cpp.push('#include "kiwi.h"');
  cpp.push("");
  if (schema.package !== null) {
    cpp.push("namespace " + schema.package + " {");
    cpp.push("");
    cpp.push("#ifndef INCLUDE_" + schema.package.toUpperCase() + "_H");
    cpp.push("#define INCLUDE_" + schema.package.toUpperCase() + "_H");
    cpp.push("");
  }
  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    definitions[definition.name] = definition;
  }
  cpp.push("class BinarySchema {");
  cpp.push("public:");
  cpp.push("  bool parse(kiwi::ByteBuffer &bb);");
  cpp.push("  const kiwi::BinarySchema &underlyingSchema() const { return _schema; }");
  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    if (definition.kind === "MESSAGE") {
      cpp.push("  bool skip" + definition.name + "Field(kiwi::ByteBuffer &bb, uint32_t id) const;");
    }
  }
  cpp.push("");
  cpp.push("private:");
  cpp.push("  kiwi::BinarySchema _schema;");
  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    if (definition.kind === "MESSAGE") {
      cpp.push("  uint32_t _index" + definition.name + " = 0;");
    }
  }
  cpp.push("};");
  cpp.push("");
  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    if (definition.kind === "ENUM") {
      cpp.push("enum class " + definition.name + " : uint32_t {");
      for (let j = 0; j < definition.fields.length; j++) {
        let field = definition.fields[j];
        cpp.push("  " + field.name + " = " + field.value + ",");
      }
      cpp.push("};");
      cpp.push("");
    } else if (definition.kind !== "STRUCT" && definition.kind !== "MESSAGE") {
      error("Invalid definition kind " + quote(definition.kind), definition.line, definition.column);
    }
  }
  for (let pass = 0; pass < 3; pass++) {
    let newline = false;
    if (pass === 2) {
      if (schema.package !== null) {
        cpp.push("#endif");
      }
      cpp.push("#ifdef IMPLEMENT_SCHEMA_H");
      cpp.push("");
      cpp.push("bool BinarySchema::parse(kiwi::ByteBuffer &bb) {");
      cpp.push("  if (!_schema.parse(bb)) return false;");
      for (let i = 0; i < schema.definitions.length; i++) {
        let definition = schema.definitions[i];
        if (definition.kind === "MESSAGE") {
          cpp.push('  _schema.findDefinition("' + definition.name + '", _index' + definition.name + ");");
        }
      }
      cpp.push("  return true;");
      cpp.push("}");
      cpp.push("");
      for (let i = 0; i < schema.definitions.length; i++) {
        let definition = schema.definitions[i];
        if (definition.kind === "MESSAGE") {
          cpp.push("bool BinarySchema::skip" + definition.name + "Field(kiwi::ByteBuffer &bb, uint32_t id) const {");
          cpp.push("  return _schema.skipField(bb, _index" + definition.name + ", id);");
          cpp.push("}");
          cpp.push("");
        }
      }
    }
    for (let i = 0; i < schema.definitions.length; i++) {
      let definition = schema.definitions[i];
      if (definition.kind === "ENUM") {
        continue;
      }
      let fields = definition.fields;
      if (pass === 0) {
        cpp.push("class " + definition.name + ";");
        newline = true;
      } else if (pass === 1) {
        cpp.push("class " + definition.name + " {");
        cpp.push("public:");
        cpp.push("  " + definition.name + "() { (void)_flags; }");
        cpp.push("");
        for (let j = 0; j < fields.length; j++) {
          let field = fields[j];
          if (field.isDeprecated) {
            continue;
          }
          let type = cppType(definitions, field, field.isArray);
          if (cppIsFieldPointer(definitions, field)) {
            cpp.push("  " + type + " *" + field.name + "();");
            cpp.push("  const " + type + " *" + field.name + "() const;");
            cpp.push("  void set_" + field.name + "(" + type + " *value);");
          } else if (field.isArray) {
            cpp.push("  " + type + " *" + field.name + "();");
            cpp.push("  const " + type + " *" + field.name + "() const;");
            cpp.push("  " + type + " &set_" + field.name + "(kiwi::MemoryPool &pool, uint32_t count);");
          } else {
            cpp.push("  " + type + " *" + field.name + "();");
            cpp.push("  const " + type + " *" + field.name + "() const;");
            cpp.push("  void set_" + field.name + "(const " + type + " &value);");
          }
          cpp.push("");
        }
        cpp.push("  bool encode(kiwi::ByteBuffer &bb);");
        cpp.push("  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);");
        cpp.push("");
        cpp.push("private:");
        cpp.push("  uint32_t _flags[" + (fields.length + 31 >> 5) + "] = {};");
        let sizes = { "bool": 1, "byte": 1, "int": 4, "uint": 4, "float": 4 };
        let sortedFields = fields.slice().sort(function(a, b) {
          let sizeA = !a.isArray && sizes[a.type] || 8;
          let sizeB = !b.isArray && sizes[b.type] || 8;
          if (sizeA !== sizeB)
            return sizeB - sizeA;
          return fields.indexOf(a) - fields.indexOf(b);
        });
        for (let j = 0; j < sortedFields.length; j++) {
          let field = sortedFields[j];
          if (field.isDeprecated) {
            continue;
          }
          let name = cppFieldName(field);
          let type = cppType(definitions, field, field.isArray);
          if (cppIsFieldPointer(definitions, field)) {
            cpp.push("  " + type + " *" + name + " = {};");
          } else {
            cpp.push("  " + type + " " + name + " = {};");
          }
        }
        cpp.push("};");
        cpp.push("");
      } else {
        for (let j = 0; j < fields.length; j++) {
          let field = fields[j];
          let name = cppFieldName(field);
          let type = cppType(definitions, field, field.isArray);
          let flagIndex = cppFlagIndex(j);
          let flagMask = cppFlagMask(j);
          if (field.isDeprecated) {
            continue;
          }
          if (cppIsFieldPointer(definitions, field)) {
            cpp.push(type + " *" + definition.name + "::" + field.name + "() {");
            cpp.push("  return " + name + ";");
            cpp.push("}");
            cpp.push("");
            cpp.push("const " + type + " *" + definition.name + "::" + field.name + "() const {");
            cpp.push("  return " + name + ";");
            cpp.push("}");
            cpp.push("");
            cpp.push("void " + definition.name + "::set_" + field.name + "(" + type + " *value) {");
            cpp.push("  " + name + " = value;");
            cpp.push("}");
            cpp.push("");
          } else if (field.isArray) {
            cpp.push(type + " *" + definition.name + "::" + field.name + "() {");
            cpp.push("  return _flags[" + flagIndex + "] & " + flagMask + " ? &" + name + " : nullptr;");
            cpp.push("}");
            cpp.push("");
            cpp.push("const " + type + " *" + definition.name + "::" + field.name + "() const {");
            cpp.push("  return _flags[" + flagIndex + "] & " + flagMask + " ? &" + name + " : nullptr;");
            cpp.push("}");
            cpp.push("");
            cpp.push(type + " &" + definition.name + "::set_" + field.name + "(kiwi::MemoryPool &pool, uint32_t count) {");
            cpp.push("  _flags[" + flagIndex + "] |= " + flagMask + "; return " + name + " = pool.array<" + cppType(definitions, field, false) + ">(count);");
            cpp.push("}");
            cpp.push("");
          } else {
            cpp.push(type + " *" + definition.name + "::" + field.name + "() {");
            cpp.push("  return _flags[" + flagIndex + "] & " + flagMask + " ? &" + name + " : nullptr;");
            cpp.push("}");
            cpp.push("");
            cpp.push("const " + type + " *" + definition.name + "::" + field.name + "() const {");
            cpp.push("  return _flags[" + flagIndex + "] & " + flagMask + " ? &" + name + " : nullptr;");
            cpp.push("}");
            cpp.push("");
            cpp.push("void " + definition.name + "::set_" + field.name + "(const " + type + " &value) {");
            cpp.push("  _flags[" + flagIndex + "] |= " + flagMask + "; " + name + " = value;");
            cpp.push("}");
            cpp.push("");
          }
        }
        cpp.push("bool " + definition.name + "::encode(kiwi::ByteBuffer &_bb) {");
        for (let j = 0; j < fields.length; j++) {
          let field = fields[j];
          if (field.isDeprecated) {
            continue;
          }
          let name = cppFieldName(field);
          let value = field.isArray ? "_it" : name;
          let flagIndex = cppFlagIndex(j);
          let flagMask = cppFlagMask(j);
          let code;
          switch (field.type) {
            case "bool": {
              code = "_bb.writeByte(" + value + ");";
              break;
            }
            case "byte": {
              code = "_bb.writeByte(" + value + ");";
              break;
            }
            case "int": {
              code = "_bb.writeVarInt(" + value + ");";
              break;
            }
            case "uint": {
              code = "_bb.writeVarUint(" + value + ");";
              break;
            }
            case "float": {
              code = "_bb.writeVarFloat(" + value + ");";
              break;
            }
            case "string": {
              code = "_bb.writeString(" + value + ".c_str());";
              break;
            }
            case "int64": {
              code = "_bb.writeVarInt64(" + value + ");";
              break;
            }
            case "uint64": {
              code = "_bb.writeVarUint64(" + value + ");";
              break;
            }
            default: {
              let type = definitions[field.type];
              if (!type) {
                error("Invalid type " + quote(field.type) + " for field " + quote(field.name), field.line, field.column);
              } else if (type.kind === "ENUM") {
                code = "_bb.writeVarUint(static_cast<uint32_t>(" + value + "));";
              } else {
                code = "if (!" + value + (cppIsFieldPointer(definitions, field) ? "->" : ".") + "encode(_bb)) return false;";
              }
            }
          }
          let indent = "  ";
          if (definition.kind === "STRUCT") {
            cpp.push("  if (" + field.name + "() == nullptr) return false;");
          } else {
            cpp.push("  if (" + field.name + "() != nullptr) {");
            indent = "    ";
          }
          if (definition.kind === "MESSAGE") {
            cpp.push(indent + "_bb.writeVarUint(" + field.value + ");");
          }
          if (field.isArray) {
            cpp.push(indent + "_bb.writeVarUint(" + name + ".size());");
            cpp.push(indent + "for (" + cppType(definitions, field, false) + " &_it : " + name + ") " + code);
          } else {
            cpp.push(indent + code);
          }
          if (definition.kind !== "STRUCT") {
            cpp.push("  }");
          }
        }
        if (definition.kind === "MESSAGE") {
          cpp.push("  _bb.writeVarUint(0);");
        }
        cpp.push("  return true;");
        cpp.push("}");
        cpp.push("");
        cpp.push("bool " + definition.name + "::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {");
        for (let j = 0; j < fields.length; j++) {
          if (fields[j].isArray) {
            cpp.push("  uint32_t _count;");
            break;
          }
        }
        if (definition.kind === "MESSAGE") {
          cpp.push("  while (true) {");
          cpp.push("    uint32_t _type;");
          cpp.push("    if (!_bb.readVarUint(_type)) return false;");
          cpp.push("    switch (_type) {");
          cpp.push("      case 0:");
          cpp.push("        return true;");
        }
        for (let j = 0; j < fields.length; j++) {
          let field = fields[j];
          let name = cppFieldName(field);
          let value = field.isArray ? "_it" : name;
          let isPointer = cppIsFieldPointer(definitions, field);
          let code;
          switch (field.type) {
            case "bool": {
              code = "_bb.readByte(" + value + ")";
              break;
            }
            case "byte": {
              code = "_bb.readByte(" + value + ")";
              break;
            }
            case "int": {
              code = "_bb.readVarInt(" + value + ")";
              break;
            }
            case "uint": {
              code = "_bb.readVarUint(" + value + ")";
              break;
            }
            case "float": {
              code = "_bb.readVarFloat(" + value + ")";
              break;
            }
            case "string": {
              code = "_bb.readString(" + value + ", _pool)";
              break;
            }
            case "int64": {
              code = "_bb.readVarInt64(" + value + ")";
              break;
            }
            case "uint64": {
              code = "_bb.readVarUint64(" + value + ")";
              break;
            }
            default: {
              let type2 = definitions[field.type];
              if (!type2) {
                error("Invalid type " + quote(field.type) + " for field " + quote(field.name), field.line, field.column);
              } else if (type2.kind === "ENUM") {
                code = "_bb.readVarUint(reinterpret_cast<uint32_t &>(" + value + "))";
              } else {
                code = value + (isPointer ? "->" : ".") + "decode(_bb, _pool, _schema)";
              }
            }
          }
          let type = cppType(definitions, field, false);
          let indent = "  ";
          if (definition.kind === "MESSAGE") {
            cpp.push("      case " + field.value + ": {");
            indent = "        ";
          }
          if (field.isArray) {
            cpp.push(indent + "if (!_bb.readVarUint(_count)) return false;");
            if (field.isDeprecated) {
              cpp.push(indent + "for (" + type + " &_it : _pool.array<" + cppType(definitions, field, false) + ">(_count)) if (!" + code + ") return false;");
            } else {
              cpp.push(indent + "for (" + type + " &_it : set_" + field.name + "(_pool, _count)) if (!" + code + ") return false;");
            }
          } else {
            if (field.isDeprecated) {
              if (isPointer) {
                cpp.push(indent + type + " *" + name + " = _pool.allocate<" + type + ">();");
              } else {
                cpp.push(indent + type + " " + name + " = {};");
              }
              cpp.push(indent + "if (!" + code + ") return false;");
            } else {
              if (isPointer) {
                cpp.push(indent + name + " = _pool.allocate<" + type + ">();");
              }
              cpp.push(indent + "if (!" + code + ") return false;");
              if (!isPointer) {
                cpp.push(indent + "set_" + field.name + "(" + name + ");");
              }
            }
          }
          if (definition.kind === "MESSAGE") {
            cpp.push("        break;");
            cpp.push("      }");
          }
        }
        if (definition.kind === "MESSAGE") {
          cpp.push("      default: {");
          cpp.push("        if (!_schema || !_schema->skip" + definition.name + "Field(_bb, _type)) return false;");
          cpp.push("        break;");
          cpp.push("      }");
          cpp.push("    }");
          cpp.push("  }");
        } else {
          cpp.push("  return true;");
        }
        cpp.push("}");
        cpp.push("");
      }
    }
    if (pass === 2) {
      cpp.push("#endif");
      cpp.push("");
    } else if (newline)
      cpp.push("");
  }
  if (schema.package !== null) {
    cpp.push("}");
    cpp.push("");
  }
  return cpp.join("\n");
}

// skew.ts
function popTrailingNewline(lines) {
  if (lines[lines.length - 1] === "") {
    lines.pop();
  }
}
function skewDefaultValueForField(definitions, field) {
  if (field.isArray) {
    return "null";
  }
  switch (field.type) {
    case "bool":
      return "false";
    case "byte":
    case "int":
    case "uint":
      return "0";
    case "float":
      return "0.0";
    case "string":
      return "null";
    case "int64":
    case "uint64":
      return "BigInt.new(0)";
  }
  let def = definitions[field.type];
  if (def.kind === "ENUM") {
    if (def.fields.length > 0) {
      return "." + def.fields[0].name;
    }
    return "0 as " + field.type;
  }
  return "null";
}
function skewTypeForField(field) {
  let type;
  switch (field.type) {
    case "bool":
      type = "bool";
      break;
    case "byte":
    case "int":
    case "uint":
      type = "int";
      break;
    case "float":
      type = "double";
      break;
    case "string":
      type = "string";
      break;
    case "int64":
    case "uint64":
      type = "BigInt";
      break;
    default:
      type = field.type;
      break;
  }
  if (field.isArray) {
    type = "List<" + type + ">";
  }
  return type;
}
function compileSchemaSkew(schema) {
  let definitions = {};
  let indent = "";
  let lines = [];
  if (schema.package !== null) {
    lines.push("namespace " + schema.package + " {");
    indent += "  ";
  }
  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    definitions[definition.name] = definition;
  }
  lines.push(indent + "class BinarySchema {");
  lines.push(indent + "  var _schema = Kiwi.BinarySchema.new");
  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    if (definition.kind === "MESSAGE") {
      lines.push(indent + "  var _index" + definition.name + " = 0");
    }
  }
  lines.push("");
  lines.push(indent + "  def parse(bytes Uint8Array) {");
  lines.push(indent + "    _schema.parse(Kiwi.ByteBuffer.new(bytes))");
  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    if (definition.kind === "MESSAGE") {
      lines.push(indent + "    _index" + definition.name + ' = _schema.findDefinition("' + definition.name + '")');
    }
  }
  lines.push(indent + "  }");
  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    if (definition.kind === "MESSAGE") {
      lines.push("");
      lines.push(indent + "  def skip" + definition.name + "Field(bb Kiwi.ByteBuffer, id int) {");
      lines.push(indent + "    _schema.skipField(bb, _index" + definition.name + ", id)");
      lines.push(indent + "  }");
    }
  }
  lines.push(indent + "}");
  lines.push("");
  for (let i = 0; i < schema.definitions.length; i++) {
    let definition = schema.definitions[i];
    switch (definition.kind) {
      case "ENUM": {
        let encode = {};
        let decode = {};
        lines.push(indent + "enum " + definition.name + " {");
        for (let j = 0; j < definition.fields.length; j++) {
          let field = definition.fields[j];
          encode[field.name] = field.value;
          decode[field.value] = field.name;
          lines.push(indent + "  " + field.name);
        }
        lines.push(indent + "}");
        lines.push("");
        lines.push(indent + "namespace " + definition.name + " {");
        lines.push(indent + "  const _encode = " + JSON.stringify(encode, null, 2).replace(/"/g, "").replace(/\n/g, "\n  " + indent));
        lines.push("");
        lines.push(indent + "  const _decode = " + JSON.stringify(decode, null, 2).replace(/"/g, "").replace(/\n/g, "\n  " + indent));
        lines.push("");
        lines.push(indent + "  def encode(value " + definition.name + ") int {");
        lines.push(indent + "    return _encode[value]");
        lines.push(indent + "  }");
        lines.push("");
        lines.push(indent + "  def decode(value int) " + definition.name + " {");
        lines.push(indent + "    if !(value in _decode) {");
        lines.push(indent + "      Kiwi.DecodeError.throwInvalidEnumValue(" + quote(definition.name) + ")");
        lines.push(indent + "    }");
        lines.push(indent + "    return _decode[value]");
        lines.push(indent + "  }");
        lines.push(indent + "}");
        lines.push("");
        break;
      }
      case "STRUCT":
      case "MESSAGE": {
        lines.push(indent + "class " + definition.name + " {");
        for (let j = 0; j < definition.fields.length; j += 32) {
          lines.push(indent + "  var _flags" + (j >> 5) + " = 0");
        }
        for (let j = 0; j < definition.fields.length; j++) {
          let field = definition.fields[j];
          if (field.isDeprecated) {
            continue;
          }
          lines.push(indent + "  var _" + field.name + " " + skewTypeForField(field) + " = " + skewDefaultValueForField(definitions, field));
        }
        lines.push("");
        for (let j = 0; j < definition.fields.length; j++) {
          let field = definition.fields[j];
          if (field.isDeprecated) {
            continue;
          }
          let type = skewTypeForField(field);
          let flags = "_flags" + (j >> 5);
          let mask = "" + (1 << j % 32 >>> 0);
          lines.push(indent + "  def has_" + field.name + " bool {");
          lines.push(indent + "    return (" + flags + " & " + mask + ") != 0");
          lines.push(indent + "  }");
          lines.push("");
          lines.push(indent + "  def " + field.name + " " + type + " {");
          lines.push(indent + "    assert(has_" + field.name + ")");
          lines.push(indent + "    return _" + field.name);
          lines.push(indent + "  }");
          lines.push("");
          lines.push(indent + "  def " + field.name + "=(value " + type + ") {");
          lines.push(indent + "    _" + field.name + " = value");
          lines.push(indent + "    " + flags + " |= " + mask);
          lines.push(indent + "  }");
          lines.push("");
        }
        lines.push(indent + "  def encode(bb Kiwi.ByteBuffer) {");
        for (let j = 0; j < definition.fields.length; j++) {
          let field = definition.fields[j];
          if (field.isDeprecated) {
            continue;
          }
          let value = "_" + field.name;
          let code;
          if (field.isArray) {
            value = "value";
          }
          switch (field.type) {
            case "bool": {
              code = "bb.writeByte(" + value + " as int)";
              break;
            }
            case "byte": {
              code = "bb.writeByte(" + value + ")";
              break;
            }
            case "int": {
              code = "bb.writeVarInt(" + value + ")";
              break;
            }
            case "uint": {
              code = "bb.writeVarUint(" + value + ")";
              break;
            }
            case "float": {
              code = "bb.writeVarFloat(" + value + ")";
              break;
            }
            case "string": {
              code = "bb.writeString(" + value + ")";
              break;
            }
            case "int64": {
              code = "bb.writeVarInt64(" + value + ")";
              break;
            }
            case "uint64": {
              code = "bb.writeVarUint64(" + value + ")";
              break;
            }
            default: {
              let type = definitions[field.type];
              if (!type) {
                error("Invalid type " + quote(field.type) + " for field " + quote(field.name), field.line, field.column);
              } else if (type.kind === "ENUM") {
                code = "bb.writeVarUint(" + type.name + ".encode(" + value + "))";
              } else {
                code = value + ".encode(bb)";
              }
            }
          }
          let nestedIndent2 = indent + "    ";
          if (definition.kind === "STRUCT") {
            lines.push(nestedIndent2 + "assert(has_" + field.name + ")");
          } else {
            lines.push(nestedIndent2 + "if has_" + field.name + " {");
            nestedIndent2 += "  ";
          }
          if (definition.kind === "MESSAGE") {
            lines.push(nestedIndent2 + "bb.writeVarUint(" + field.value + ")");
          }
          if (field.isArray) {
            lines.push(nestedIndent2 + "bb.writeVarUint(_" + field.name + ".count)");
            lines.push(nestedIndent2 + "for value in _" + field.name + " {");
            lines.push(nestedIndent2 + "  " + code);
            lines.push(nestedIndent2 + "}");
          } else {
            lines.push(nestedIndent2 + code);
          }
          if (definition.kind !== "STRUCT") {
            lines.push(indent + "    }");
          }
          lines.push("");
        }
        if (definition.kind === "MESSAGE") {
          lines.push(indent + "    bb.writeVarUint(0)");
        } else {
          popTrailingNewline(lines);
        }
        lines.push(indent + "  }");
        lines.push("");
        lines.push(indent + "  def encode Uint8Array {");
        lines.push(indent + "    var bb = Kiwi.ByteBuffer.new");
        lines.push(indent + "    encode(bb)");
        lines.push(indent + "    return bb.toUint8Array");
        lines.push(indent + "  }");
        lines.push(indent + "}");
        lines.push("");
        lines.push(indent + "namespace " + definition.name + " {");
        lines.push(indent + "  def decode(bytes Uint8Array) " + definition.name + " {");
        lines.push(indent + "    return decode(Kiwi.ByteBuffer.new(bytes), null)");
        lines.push(indent + "  }");
        lines.push("");
        lines.push(indent + "  def decode(bytes Uint8Array, schema BinarySchema) " + definition.name + " {");
        lines.push(indent + "    return decode(Kiwi.ByteBuffer.new(bytes), schema)");
        lines.push(indent + "  }");
        lines.push("");
        lines.push(indent + "  def decode(bb Kiwi.ByteBuffer, schema BinarySchema) " + definition.name + " {");
        lines.push(indent + "    var self = new");
        for (let j = 0; j < definition.fields.length; j++) {
          if (definition.fields[j].isArray) {
            lines.push(indent + "    var count = 0");
            break;
          }
        }
        let nestedIndent = indent + "  ";
        if (definition.kind === "MESSAGE") {
          lines.push(indent + "    while true {");
          lines.push(indent + "      var type = bb.readVarUint");
          lines.push(indent + "      switch type {");
          lines.push(indent + "        case 0 {");
          lines.push(indent + "          break");
          lines.push(indent + "        }");
          lines.push("");
          nestedIndent += "      ";
        }
        for (let j = 0; j < definition.fields.length; j++) {
          let field = definition.fields[j];
          let code;
          switch (field.type) {
            case "bool": {
              code = field.isDeprecated ? "bb.readByte" : "bb.readByte as bool";
              break;
            }
            case "byte": {
              code = "bb.readByte";
              break;
            }
            case "int": {
              code = "bb.readVarInt";
              break;
            }
            case "uint": {
              code = "bb.readVarUint";
              break;
            }
            case "float": {
              code = "bb.readVarFloat";
              break;
            }
            case "string": {
              code = "bb.readString";
              break;
            }
            case "int64": {
              code = "bb.readVarInt64";
              break;
            }
            case "uint64": {
              code = "bb.readVarUint64";
              break;
            }
            default: {
              let type = definitions[field.type];
              if (!type) {
                error("Invalid type " + quote(field.type) + " for field " + quote(field.name), field.line, field.column);
              } else if (type.kind === "ENUM") {
                code = type.name + ".decode(bb.readVarUint)";
              } else {
                code = type.name + ".decode(bb, schema)";
              }
            }
          }
          if (definition.kind === "MESSAGE") {
            lines.push(nestedIndent + "case " + field.value + " {");
          }
          if (field.isArray) {
            if (field.isDeprecated) {
              lines.push(nestedIndent + "  for i in 0..bb.readVarUint {");
              lines.push(nestedIndent + "    " + code);
              lines.push(nestedIndent + "  }");
            } else {
              lines.push(nestedIndent + "  count = bb.readVarUint");
              lines.push(nestedIndent + "  self." + field.name + " = []");
              lines.push(nestedIndent + "  for array = self._" + field.name + "; count != 0; count-- {");
              lines.push(nestedIndent + "    array.append(" + code + ")");
              lines.push(nestedIndent + "  }");
            }
          } else {
            if (field.isDeprecated) {
              lines.push(nestedIndent + "  " + code);
            } else {
              lines.push(nestedIndent + "  self." + field.name + " = " + code);
            }
          }
          if (definition.kind === "MESSAGE") {
            lines.push(nestedIndent + "}");
            lines.push("");
          }
        }
        if (definition.kind === "MESSAGE") {
          lines.push(indent + "        default {");
          lines.push(indent + "          if schema == null { Kiwi.DecodeError.throwInvalidMessage }");
          lines.push(indent + "          else { schema.skip" + definition.name + "Field(bb, type) }");
          lines.push(indent + "        }");
          lines.push(indent + "      }");
          lines.push(indent + "    }");
        }
        lines.push(indent + "    return self");
        lines.push(indent + "  }");
        lines.push(indent + "}");
        lines.push("");
        break;
      }
      default: {
        error("Invalid definition kind " + quote(definition.kind), definition.line, definition.column);
        break;
      }
    }
  }
  if (schema.package !== null) {
    popTrailingNewline(lines);
    lines.push("}");
  }
  lines.push("");
  return lines.join("\n");
}

// skew-types.ts
function compileSchemaSkewTypes(schema) {
  var indent = "";
  var lines = [];
  if (schema.package !== null) {
    lines.push("namespace " + schema.package + " {");
    indent += "  ";
  }
  for (var i = 0; i < schema.definitions.length; i++) {
    var definition = schema.definitions[i];
    if (definition.kind === "ENUM") {
      lines.push(indent + "type " + definition.name + " : string {");
      lines.push(indent + "  @alwaysinline");
      lines.push(indent + "  def toString string { return self as string }");
      lines.push(indent + "}");
      if (definition.fields.length > 0) {
        lines.push(indent + "namespace " + definition.name + " {");
        for (var j = 0; j < definition.fields.length; j++) {
          lines.push(indent + "  @alwaysinline");
          lines.push(indent + "  def " + definition.fields[j].name + " " + definition.name + " { return " + JSON.stringify(definition.fields[j].name) + " as " + definition.name + " }");
        }
        lines.push(indent + "}");
      }
      lines.push("");
    }
  }
  for (var i = 0; i < schema.definitions.length; i++) {
    var definition = schema.definitions[i];
    if (definition.kind === "STRUCT" || definition.kind === "MESSAGE") {
      lines.push(indent + "@import");
      lines.push(indent + "class " + definition.name + " {");
      for (var j = 0; j < definition.fields.length; j++) {
        var field = definition.fields[j];
        var type;
        if (field.isDeprecated) {
          continue;
        }
        switch (field.type) {
          case "byte":
          case "uint":
            type = "int";
            break;
          case "float":
            type = "double";
            break;
          case "int64":
          case "uint64":
            type = "BigInt";
            break;
          default:
            type = field.type;
            break;
        }
        if (field.type === "byte" && field.isArray)
          type = "Uint8Array";
        else if (field.isArray)
          type = "List<" + type + ">";
        lines.push(indent + "  var " + field.name + " " + type);
        lines.push(indent + "  @alwaysinline");
        lines.push(indent + "  def has_" + field.name + " bool { return self." + field.name + " != dynamic.void(0) }");
      }
      lines.push(indent + "}");
      lines.push("");
      lines.push(indent + "namespace " + definition.name + " {");
      lines.push(indent + "  @alwaysinline");
      lines.push(indent + "  def new " + definition.name + " { return {} as dynamic }");
      lines.push(indent + "}");
      lines.push("");
    } else if (definition.kind !== "ENUM") {
      error("Invalid definition kind " + quote(definition.kind), definition.line, definition.column);
    }
  }
  lines.push(indent + "@import");
  lines.push(indent + "class Schema {");
  for (var i = 0; i < schema.definitions.length; i++) {
    var definition = schema.definitions[i];
    if (definition.kind === "ENUM") {
      lines.push(indent + "  const " + definition.name + " dynamic");
    } else if (definition.kind === "STRUCT" || definition.kind === "MESSAGE") {
      lines.push(indent + "  def encode" + definition.name + "(message " + definition.name + ") Uint8Array");
      lines.push(indent + "  def decode" + definition.name + "(buffer Uint8Array) " + definition.name);
    }
  }
  lines.push(indent + "}");
  if (schema.package !== null) {
    lines.push("}");
  }
  lines.push("");
  return lines.join("\n");
}

// ts.ts
function compileSchemaTypeScript(schema) {
  var indent = "";
  var lines = [];
  if (schema.package !== null) {
    lines.push("export namespace " + schema.package + " {");
    indent += "  ";
  }
  for (var i = 0; i < schema.definitions.length; i++) {
    var definition = schema.definitions[i];
    if (definition.kind === "ENUM") {
      lines.push(indent + "export type " + definition.name + " =");
      for (var j = 0; j < definition.fields.length; j++) {
        lines.push(indent + "  " + JSON.stringify(definition.fields[j].name) + (j + 1 < definition.fields.length ? " |" : ";"));
      }
      if (!definition.fields.length) {
        lines.push(indent + "  any;");
      }
      lines.push("");
    }
  }
  for (var i = 0; i < schema.definitions.length; i++) {
    var definition = schema.definitions[i];
    if (definition.kind === "STRUCT" || definition.kind === "MESSAGE") {
      lines.push(indent + "export interface " + definition.name + " {");
      for (var j = 0; j < definition.fields.length; j++) {
        var field = definition.fields[j];
        var type;
        if (field.isDeprecated) {
          continue;
        }
        switch (field.type) {
          case "bool":
            type = "boolean";
            break;
          case "byte":
          case "int":
          case "uint":
          case "float":
            type = "number";
            break;
          case "int64":
          case "uint64":
            type = "bigint";
            break;
          default:
            type = field.type;
            break;
        }
        if (field.type === "byte" && field.isArray)
          type = "Uint8Array";
        else if (field.isArray)
          type += "[]";
        lines.push(indent + "  " + field.name + (definition.kind === "MESSAGE" ? "?" : "") + ": " + type + ";");
      }
      lines.push(indent + "}");
      lines.push("");
    } else if (definition.kind !== "ENUM") {
      error("Invalid definition kind " + quote(definition.kind), definition.line, definition.column);
    }
  }
  lines.push(indent + "export interface Schema {");
  for (var i = 0; i < schema.definitions.length; i++) {
    var definition = schema.definitions[i];
    if (definition.kind === "ENUM") {
      lines.push(indent + "  " + definition.name + ": any;");
    } else if (definition.kind === "STRUCT" || definition.kind === "MESSAGE") {
      lines.push(indent + "  encode" + definition.name + "(message: " + definition.name + "): Uint8Array;");
      lines.push(indent + "  decode" + definition.name + "(buffer: Uint8Array): " + definition.name + ";");
    }
  }
  lines.push(indent + "}");
  if (schema.package !== null) {
    lines.push("}");
  }
  lines.push("");
  return lines.join("\n");
}

// binary.ts
var types = ["bool", "byte", "int", "uint", "float", "string", "int64", "uint64"];
var kinds = ["ENUM", "STRUCT", "MESSAGE"];
function decodeBinarySchema(buffer) {
  let bb = buffer instanceof ByteBuffer ? buffer : new ByteBuffer(buffer);
  let definitionCount = bb.readVarUint();
  let definitions = [];
  for (let i = 0; i < definitionCount; i++) {
    let definitionName = bb.readString();
    let kind = bb.readByte();
    let fieldCount = bb.readVarUint();
    let fields = [];
    for (let j = 0; j < fieldCount; j++) {
      let fieldName = bb.readString();
      let type = bb.readVarInt();
      let isArray = !!(bb.readByte() & 1);
      let value = bb.readVarUint();
      fields.push({
        name: fieldName,
        line: 0,
        column: 0,
        type: kinds[kind] === "ENUM" ? null : type,
        isArray,
        isDeprecated: false,
        value
      });
    }
    definitions.push({
      name: definitionName,
      line: 0,
      column: 0,
      kind: kinds[kind],
      fields
    });
  }
  for (let i = 0; i < definitionCount; i++) {
    let fields = definitions[i].fields;
    for (let j = 0; j < fields.length; j++) {
      let field = fields[j];
      let type = field.type;
      if (type !== null && type < 0) {
        if (~type >= types.length) {
          throw new Error("Invalid type " + type);
        }
        field.type = types[~type];
      } else {
        if (type !== null && type >= definitions.length) {
          throw new Error("Invalid type " + type);
        }
        field.type = type === null ? null : definitions[type].name;
      }
    }
  }
  return {
    package: null,
    definitions
  };
}
function encodeBinarySchema(schema) {
  let bb = new ByteBuffer();
  let definitions = schema.definitions;
  let definitionIndex = {};
  bb.writeVarUint(definitions.length);
  for (let i = 0; i < definitions.length; i++) {
    definitionIndex[definitions[i].name] = i;
  }
  for (let i = 0; i < definitions.length; i++) {
    let definition = definitions[i];
    bb.writeString(definition.name);
    bb.writeByte(kinds.indexOf(definition.kind));
    bb.writeVarUint(definition.fields.length);
    for (let j = 0; j < definition.fields.length; j++) {
      let field = definition.fields[j];
      let type = types.indexOf(field.type);
      bb.writeString(field.name);
      bb.writeVarInt(type === -1 ? definitionIndex[field.type] : ~type);
      bb.writeByte(field.isArray ? 1 : 0);
      bb.writeVarUint(field.value);
    }
  }
  return bb.toUint8Array();
}

// parser.ts
var nativeTypes = [
  "bool",
  "byte",
  "float",
  "int",
  "int64",
  "string",
  "uint",
  "uint64"
];
var reservedNames = [
  "ByteBuffer",
  "package"
];
var regex = /((?:-|\b)\d+\b|[=;{}]|\[\]|\[deprecated\]|\b[A-Za-z_][A-Za-z0-9_]*\b|\/\/.*|\s+)/g;
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
var deprecatedToken = /^\[deprecated\]$/;
function tokenize(text) {
  let parts = text.split(regex);
  let tokens = [];
  let column = 0;
  let line = 0;
  for (let i = 0; i < parts.length; i++) {
    let part = parts[i];
    if (i & 1) {
      if (!whitespace.test(part)) {
        tokens.push({
          text: part,
          line: line + 1,
          column: column + 1
        });
      }
    } else if (part !== "") {
      error("Syntax error " + quote(part), line + 1, column + 1);
    }
    let lines = part.split("\n");
    if (lines.length > 1)
      column = 0;
    line += lines.length - 1;
    column += lines[lines.length - 1].length;
  }
  tokens.push({
    text: "",
    line,
    column
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
      let token = current();
      error("Expected " + expected + " but found " + quote(token.text), token.line, token.column);
    }
  }
  function unexpectedToken() {
    let token = current();
    error("Unexpected token " + quote(token.text), token.line, token.column);
  }
  let definitions = [];
  let packageText = null;
  let index = 0;
  if (eat(packageKeyword)) {
    packageText = current().text;
    expect(identifier, "identifier");
    expect(semicolon, '";"');
  }
  while (index < tokens.length && !eat(endOfFile)) {
    let fields = [];
    let kind;
    if (eat(enumKeyword))
      kind = "ENUM";
    else if (eat(structKeyword))
      kind = "STRUCT";
    else if (eat(messageKeyword))
      kind = "MESSAGE";
    else
      unexpectedToken();
    let name = current();
    expect(identifier, "identifier");
    expect(leftBrace, '"{"');
    while (!eat(rightBrace)) {
      let type = null;
      let isArray = false;
      let isDeprecated = false;
      if (kind !== "ENUM") {
        type = current().text;
        expect(identifier, "identifier");
        isArray = eat(arrayToken);
      }
      let field = current();
      expect(identifier, "identifier");
      let value = null;
      if (kind !== "STRUCT") {
        expect(equals, '"="');
        value = current();
        expect(integer, "integer");
        if ((+value.text | 0) + "" !== value.text) {
          error("Invalid integer " + quote(value.text), value.line, value.column);
        }
      }
      let deprecated = current();
      if (eat(deprecatedToken)) {
        if (kind !== "MESSAGE") {
          error("Cannot deprecate this field", deprecated.line, deprecated.column);
        }
        isDeprecated = true;
      }
      expect(semicolon, '";"');
      fields.push({
        name: field.text,
        line: field.line,
        column: field.column,
        type,
        isArray,
        isDeprecated,
        value: value !== null ? +value.text | 0 : fields.length + 1
      });
    }
    definitions.push({
      name: name.text,
      line: name.line,
      column: name.column,
      kind,
      fields
    });
  }
  return {
    package: packageText,
    definitions
  };
}
function verify(root) {
  let definedTypes = nativeTypes.slice();
  let definitions = {};
  for (let i = 0; i < root.definitions.length; i++) {
    let definition = root.definitions[i];
    if (definedTypes.indexOf(definition.name) !== -1) {
      error("The type " + quote(definition.name) + " is defined twice", definition.line, definition.column);
    }
    if (reservedNames.indexOf(definition.name) !== -1) {
      error("The type name " + quote(definition.name) + " is reserved", definition.line, definition.column);
    }
    definedTypes.push(definition.name);
    definitions[definition.name] = definition;
  }
  for (let i = 0; i < root.definitions.length; i++) {
    let definition = root.definitions[i];
    let fields = definition.fields;
    if (definition.kind === "ENUM" || fields.length === 0) {
      continue;
    }
    for (let j = 0; j < fields.length; j++) {
      let field = fields[j];
      if (definedTypes.indexOf(field.type) === -1) {
        error("The type " + quote(field.type) + " is not defined for field " + quote(field.name), field.line, field.column);
      }
    }
    let values = [];
    for (let j = 0; j < fields.length; j++) {
      let field = fields[j];
      if (values.indexOf(field.value) !== -1) {
        error("The id for field " + quote(field.name) + " is used twice", field.line, field.column);
      }
      if (field.value <= 0) {
        error("The id for field " + quote(field.name) + " must be positive", field.line, field.column);
      }
      if (field.value > fields.length) {
        error("The id for field " + quote(field.name) + " cannot be larger than " + fields.length, field.line, field.column);
      }
      values.push(field.value);
    }
  }
  let state = {};
  let check = (name) => {
    let definition = definitions[name];
    if (definition && definition.kind === "STRUCT") {
      if (state[name] === 1) {
        error("Recursive nesting of " + quote(name) + " is not allowed", definition.line, definition.column);
      }
      if (state[name] !== 2 && definition) {
        state[name] = 1;
        let fields = definition.fields;
        for (let i = 0; i < fields.length; i++) {
          let field = fields[i];
          if (!field.isArray) {
            check(field.type);
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
function parseSchema(text) {
  let schema = parse(tokenize(text));
  verify(schema);
  return schema;
}

// printer.ts
function prettyPrintSchema(schema) {
  let definitions = schema.definitions;
  let text = "";
  if (schema.package !== null) {
    text += "package " + schema.package + ";\n";
  }
  for (let i = 0; i < definitions.length; i++) {
    let definition = definitions[i];
    if (i > 0 || schema.package !== null)
      text += "\n";
    text += definition.kind.toLowerCase() + " " + definition.name + " {\n";
    for (let j = 0; j < definition.fields.length; j++) {
      let field = definition.fields[j];
      text += "  ";
      if (definition.kind !== "ENUM") {
        text += field.type;
        if (field.isArray) {
          text += "[]";
        }
        text += " ";
      }
      text += field.name;
      if (definition.kind !== "STRUCT") {
        text += " = " + field.value;
      }
      if (field.isDeprecated) {
        text += " [deprecated]";
      }
      text += ";\n";
    }
    text += "}\n";
  }
  return text;
}
export {
  ByteBuffer,
  compileSchema,
  compileSchemaCPP,
  compileSchemaCallbackCPP,
  compileSchemaJS,
  compileSchemaSkew,
  compileSchemaSkewTypes,
  compileSchemaTypeScript,
  decodeBinarySchema,
  encodeBinarySchema,
  parseSchema,
  prettyPrintSchema
};
