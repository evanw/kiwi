#include "kiwi.h"

namespace test {

class BoolStruct;
class ByteStruct;
class IntStruct;
class UintStruct;
class FloatStruct;
class StringStruct;
class CompoundStruct;
class NestedStruct;
class BoolMessage;
class ByteMessage;
class IntMessage;
class UintMessage;
class FloatMessage;
class StringMessage;
class CompoundMessage;
class NestedMessage;
class BoolArrayStruct;
class ByteArrayStruct;
class IntArrayStruct;
class UintArrayStruct;
class FloatArrayStruct;
class StringArrayStruct;
class CompoundArrayStruct;
class BoolArrayMessage;
class ByteArrayMessage;
class IntArrayMessage;
class UintArrayMessage;
class FloatArrayMessage;
class StringArrayMessage;
class CompoundArrayMessage;
class RequiredField;

class BoolStruct {
public:
  bool *x() { return _x; }
  bool &add_x(kiwi::MemoryPool &pool) { return *(_x ? _x : _x = pool.allocate<bool>(1)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  bool *_x = nullptr;
};

class ByteStruct {
public:
  uint8_t *x() { return _x; }
  uint8_t &add_x(kiwi::MemoryPool &pool) { return *(_x ? _x : _x = pool.allocate<uint8_t>(1)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint8_t *_x = nullptr;
};

class IntStruct {
public:
  int32_t *x() { return _x; }
  int32_t &add_x(kiwi::MemoryPool &pool) { return *(_x ? _x : _x = pool.allocate<int32_t>(1)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  int32_t *_x = nullptr;
};

class UintStruct {
public:
  uint32_t *x() { return _x; }
  uint32_t &add_x(kiwi::MemoryPool &pool) { return *(_x ? _x : _x = pool.allocate<uint32_t>(1)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t *_x = nullptr;
};

class FloatStruct {
public:
  float *x() { return _x; }
  float &add_x(kiwi::MemoryPool &pool) { return *(_x ? _x : _x = pool.allocate<float>(1)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  float *_x = nullptr;
};

class StringStruct {
public:
  kiwi::String *x() { return _x; }
  kiwi::String &add_x(kiwi::MemoryPool &pool) { return *(_x ? _x : _x = pool.allocate<kiwi::String>(1)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  kiwi::String *_x = nullptr;
};

class CompoundStruct {
public:
  uint32_t *x() { return _x; }
  uint32_t &add_x(kiwi::MemoryPool &pool) { return *(_x ? _x : _x = pool.allocate<uint32_t>(1)); }

  uint32_t *y() { return _y; }
  uint32_t &add_y(kiwi::MemoryPool &pool) { return *(_y ? _y : _y = pool.allocate<uint32_t>(1)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t *_x = nullptr;
  uint32_t *_y = nullptr;
};

class NestedStruct {
public:
  uint32_t *a() { return _a; }
  uint32_t &add_a(kiwi::MemoryPool &pool) { return *(_a ? _a : _a = pool.allocate<uint32_t>(1)); }

  CompoundStruct *b() { return _b; }
  CompoundStruct &add_b(kiwi::MemoryPool &pool) { return *(_b ? _b : _b = pool.allocate<CompoundStruct>(1)); }

  uint32_t *c() { return _c; }
  uint32_t &add_c(kiwi::MemoryPool &pool) { return *(_c ? _c : _c = pool.allocate<uint32_t>(1)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t *_a = nullptr;
  CompoundStruct *_b = nullptr;
  uint32_t *_c = nullptr;
};

class BoolMessage {
public:
  bool *x() { return _x; }
  bool &add_x(kiwi::MemoryPool &pool) { return *(_x ? _x : _x = pool.allocate<bool>(1)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  bool *_x = nullptr;
};

class ByteMessage {
public:
  uint8_t *x() { return _x; }
  uint8_t &add_x(kiwi::MemoryPool &pool) { return *(_x ? _x : _x = pool.allocate<uint8_t>(1)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint8_t *_x = nullptr;
};

class IntMessage {
public:
  int32_t *x() { return _x; }
  int32_t &add_x(kiwi::MemoryPool &pool) { return *(_x ? _x : _x = pool.allocate<int32_t>(1)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  int32_t *_x = nullptr;
};

class UintMessage {
public:
  uint32_t *x() { return _x; }
  uint32_t &add_x(kiwi::MemoryPool &pool) { return *(_x ? _x : _x = pool.allocate<uint32_t>(1)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t *_x = nullptr;
};

class FloatMessage {
public:
  float *x() { return _x; }
  float &add_x(kiwi::MemoryPool &pool) { return *(_x ? _x : _x = pool.allocate<float>(1)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  float *_x = nullptr;
};

class StringMessage {
public:
  kiwi::String *x() { return _x; }
  kiwi::String &add_x(kiwi::MemoryPool &pool) { return *(_x ? _x : _x = pool.allocate<kiwi::String>(1)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  kiwi::String *_x = nullptr;
};

class CompoundMessage {
public:
  uint32_t *x() { return _x; }
  uint32_t &add_x(kiwi::MemoryPool &pool) { return *(_x ? _x : _x = pool.allocate<uint32_t>(1)); }

  uint32_t *y() { return _y; }
  uint32_t &add_y(kiwi::MemoryPool &pool) { return *(_y ? _y : _y = pool.allocate<uint32_t>(1)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t *_x = nullptr;
  uint32_t *_y = nullptr;
};

class NestedMessage {
public:
  uint32_t *a() { return _a; }
  uint32_t &add_a(kiwi::MemoryPool &pool) { return *(_a ? _a : _a = pool.allocate<uint32_t>(1)); }

  CompoundMessage *b() { return _b; }
  CompoundMessage &add_b(kiwi::MemoryPool &pool) { return *(_b ? _b : _b = pool.allocate<CompoundMessage>(1)); }

  uint32_t *c() { return _c; }
  uint32_t &add_c(kiwi::MemoryPool &pool) { return *(_c ? _c : _c = pool.allocate<uint32_t>(1)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t *_a = nullptr;
  CompoundMessage *_b = nullptr;
  uint32_t *_c = nullptr;
};

class BoolArrayStruct {
public:
  kiwi::Array<bool> *x() { return _x; }
  kiwi::Array<bool> &add_x(kiwi::MemoryPool &pool, uint32_t count) { return *(_x = pool.array<bool>(count)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  kiwi::Array<bool> *_x = nullptr;
};

class ByteArrayStruct {
public:
  kiwi::Array<uint8_t> *x() { return _x; }
  kiwi::Array<uint8_t> &add_x(kiwi::MemoryPool &pool, uint32_t count) { return *(_x = pool.array<uint8_t>(count)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  kiwi::Array<uint8_t> *_x = nullptr;
};

class IntArrayStruct {
public:
  kiwi::Array<int32_t> *x() { return _x; }
  kiwi::Array<int32_t> &add_x(kiwi::MemoryPool &pool, uint32_t count) { return *(_x = pool.array<int32_t>(count)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  kiwi::Array<int32_t> *_x = nullptr;
};

class UintArrayStruct {
public:
  kiwi::Array<uint32_t> *x() { return _x; }
  kiwi::Array<uint32_t> &add_x(kiwi::MemoryPool &pool, uint32_t count) { return *(_x = pool.array<uint32_t>(count)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  kiwi::Array<uint32_t> *_x = nullptr;
};

class FloatArrayStruct {
public:
  kiwi::Array<float> *x() { return _x; }
  kiwi::Array<float> &add_x(kiwi::MemoryPool &pool, uint32_t count) { return *(_x = pool.array<float>(count)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  kiwi::Array<float> *_x = nullptr;
};

class StringArrayStruct {
public:
  kiwi::Array<kiwi::String> *x() { return _x; }
  kiwi::Array<kiwi::String> &add_x(kiwi::MemoryPool &pool, uint32_t count) { return *(_x = pool.array<kiwi::String>(count)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  kiwi::Array<kiwi::String> *_x = nullptr;
};

class CompoundArrayStruct {
public:
  kiwi::Array<uint32_t> *x() { return _x; }
  kiwi::Array<uint32_t> &add_x(kiwi::MemoryPool &pool, uint32_t count) { return *(_x = pool.array<uint32_t>(count)); }

  kiwi::Array<uint32_t> *y() { return _y; }
  kiwi::Array<uint32_t> &add_y(kiwi::MemoryPool &pool, uint32_t count) { return *(_y = pool.array<uint32_t>(count)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  kiwi::Array<uint32_t> *_x = nullptr;
  kiwi::Array<uint32_t> *_y = nullptr;
};

class BoolArrayMessage {
public:
  kiwi::Array<bool> *x() { return _x; }
  kiwi::Array<bool> &add_x(kiwi::MemoryPool &pool, uint32_t count) { return *(_x = pool.array<bool>(count)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  kiwi::Array<bool> *_x = nullptr;
};

class ByteArrayMessage {
public:
  kiwi::Array<uint8_t> *x() { return _x; }
  kiwi::Array<uint8_t> &add_x(kiwi::MemoryPool &pool, uint32_t count) { return *(_x = pool.array<uint8_t>(count)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  kiwi::Array<uint8_t> *_x = nullptr;
};

class IntArrayMessage {
public:
  kiwi::Array<int32_t> *x() { return _x; }
  kiwi::Array<int32_t> &add_x(kiwi::MemoryPool &pool, uint32_t count) { return *(_x = pool.array<int32_t>(count)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  kiwi::Array<int32_t> *_x = nullptr;
};

class UintArrayMessage {
public:
  kiwi::Array<uint32_t> *x() { return _x; }
  kiwi::Array<uint32_t> &add_x(kiwi::MemoryPool &pool, uint32_t count) { return *(_x = pool.array<uint32_t>(count)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  kiwi::Array<uint32_t> *_x = nullptr;
};

class FloatArrayMessage {
public:
  kiwi::Array<float> *x() { return _x; }
  kiwi::Array<float> &add_x(kiwi::MemoryPool &pool, uint32_t count) { return *(_x = pool.array<float>(count)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  kiwi::Array<float> *_x = nullptr;
};

class StringArrayMessage {
public:
  kiwi::Array<kiwi::String> *x() { return _x; }
  kiwi::Array<kiwi::String> &add_x(kiwi::MemoryPool &pool, uint32_t count) { return *(_x = pool.array<kiwi::String>(count)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  kiwi::Array<kiwi::String> *_x = nullptr;
};

class CompoundArrayMessage {
public:
  kiwi::Array<uint32_t> *x() { return _x; }
  kiwi::Array<uint32_t> &add_x(kiwi::MemoryPool &pool, uint32_t count) { return *(_x = pool.array<uint32_t>(count)); }

  kiwi::Array<uint32_t> *y() { return _y; }
  kiwi::Array<uint32_t> &add_y(kiwi::MemoryPool &pool, uint32_t count) { return *(_y = pool.array<uint32_t>(count)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  kiwi::Array<uint32_t> *_x = nullptr;
  kiwi::Array<uint32_t> *_y = nullptr;
};

class RequiredField {
public:
  int32_t *x() { return _x; }
  int32_t &add_x(kiwi::MemoryPool &pool) { return *(_x ? _x : _x = pool.allocate<int32_t>(1)); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  int32_t *_x = nullptr;
};

bool BoolStruct::encode(kiwi::ByteBuffer &bb) {
  if (!_x) return false;
  bb.writeByte(*_x);
  return true;
}

bool BoolStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  if (!bb.readByte(add_x(pool))) return false;
  return true;
}

bool ByteStruct::encode(kiwi::ByteBuffer &bb) {
  if (!_x) return false;
  bb.writeByte(*_x);
  return true;
}

bool ByteStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  if (!bb.readByte(add_x(pool))) return false;
  return true;
}

bool IntStruct::encode(kiwi::ByteBuffer &bb) {
  if (!_x) return false;
  bb.writeVarInt(*_x);
  return true;
}

bool IntStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  if (!bb.readVarInt(add_x(pool))) return false;
  return true;
}

bool UintStruct::encode(kiwi::ByteBuffer &bb) {
  if (!_x) return false;
  bb.writeVarUint(*_x);
  return true;
}

bool UintStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  if (!bb.readVarUint(add_x(pool))) return false;
  return true;
}

bool FloatStruct::encode(kiwi::ByteBuffer &bb) {
  if (!_x) return false;
  bb.writeFloat(*_x);
  return true;
}

bool FloatStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  if (!bb.readFloat(add_x(pool))) return false;
  return true;
}

bool StringStruct::encode(kiwi::ByteBuffer &bb) {
  if (!_x) return false;
  bb.writeString((*_x).c_str());
  return true;
}

bool StringStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  if (!bb.readString(add_x(pool), pool)) return false;
  return true;
}

bool CompoundStruct::encode(kiwi::ByteBuffer &bb) {
  if (!_x) return false;
  bb.writeVarUint(*_x);
  if (!_y) return false;
  bb.writeVarUint(*_y);
  return true;
}

bool CompoundStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  if (!bb.readVarUint(add_x(pool))) return false;
  if (!bb.readVarUint(add_y(pool))) return false;
  return true;
}

bool NestedStruct::encode(kiwi::ByteBuffer &bb) {
  if (!_a) return false;
  bb.writeVarUint(*_a);
  if (!_b) return false;
  if (!(*_b).encode(bb)) return false;
  if (!_c) return false;
  bb.writeVarUint(*_c);
  return true;
}

bool NestedStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  if (!bb.readVarUint(add_a(pool))) return false;
  if (!add_b(pool).decode(bb, pool)) return false;
  if (!bb.readVarUint(add_c(pool))) return false;
  return true;
}

bool BoolMessage::encode(kiwi::ByteBuffer &bb) {
  if (_x) {
    bb.writeVarUint(1);
    bb.writeByte(*_x);
  }
  bb.writeVarUint(0);
  return true;
}

bool BoolMessage::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  while (true) {
    uint32_t type;
    if (!bb.readVarUint(type)) return false;
    switch (type) {
      case 0:
        return true;
      case 1:
        if (!bb.readByte(add_x(pool))) return false;
        break;
      default: return false;
    }
  }
}

bool ByteMessage::encode(kiwi::ByteBuffer &bb) {
  if (_x) {
    bb.writeVarUint(1);
    bb.writeByte(*_x);
  }
  bb.writeVarUint(0);
  return true;
}

bool ByteMessage::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  while (true) {
    uint32_t type;
    if (!bb.readVarUint(type)) return false;
    switch (type) {
      case 0:
        return true;
      case 1:
        if (!bb.readByte(add_x(pool))) return false;
        break;
      default: return false;
    }
  }
}

bool IntMessage::encode(kiwi::ByteBuffer &bb) {
  if (_x) {
    bb.writeVarUint(1);
    bb.writeVarInt(*_x);
  }
  bb.writeVarUint(0);
  return true;
}

bool IntMessage::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  while (true) {
    uint32_t type;
    if (!bb.readVarUint(type)) return false;
    switch (type) {
      case 0:
        return true;
      case 1:
        if (!bb.readVarInt(add_x(pool))) return false;
        break;
      default: return false;
    }
  }
}

bool UintMessage::encode(kiwi::ByteBuffer &bb) {
  if (_x) {
    bb.writeVarUint(1);
    bb.writeVarUint(*_x);
  }
  bb.writeVarUint(0);
  return true;
}

bool UintMessage::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  while (true) {
    uint32_t type;
    if (!bb.readVarUint(type)) return false;
    switch (type) {
      case 0:
        return true;
      case 1:
        if (!bb.readVarUint(add_x(pool))) return false;
        break;
      default: return false;
    }
  }
}

bool FloatMessage::encode(kiwi::ByteBuffer &bb) {
  if (_x) {
    bb.writeVarUint(1);
    bb.writeFloat(*_x);
  }
  bb.writeVarUint(0);
  return true;
}

bool FloatMessage::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  while (true) {
    uint32_t type;
    if (!bb.readVarUint(type)) return false;
    switch (type) {
      case 0:
        return true;
      case 1:
        if (!bb.readFloat(add_x(pool))) return false;
        break;
      default: return false;
    }
  }
}

bool StringMessage::encode(kiwi::ByteBuffer &bb) {
  if (_x) {
    bb.writeVarUint(1);
    bb.writeString((*_x).c_str());
  }
  bb.writeVarUint(0);
  return true;
}

bool StringMessage::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  while (true) {
    uint32_t type;
    if (!bb.readVarUint(type)) return false;
    switch (type) {
      case 0:
        return true;
      case 1:
        if (!bb.readString(add_x(pool), pool)) return false;
        break;
      default: return false;
    }
  }
}

bool CompoundMessage::encode(kiwi::ByteBuffer &bb) {
  if (_x) {
    bb.writeVarUint(1);
    bb.writeVarUint(*_x);
  }
  if (_y) {
    bb.writeVarUint(2);
    bb.writeVarUint(*_y);
  }
  bb.writeVarUint(0);
  return true;
}

bool CompoundMessage::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  while (true) {
    uint32_t type;
    if (!bb.readVarUint(type)) return false;
    switch (type) {
      case 0:
        return true;
      case 1:
        if (!bb.readVarUint(add_x(pool))) return false;
        break;
      case 2:
        if (!bb.readVarUint(add_y(pool))) return false;
        break;
      default: return false;
    }
  }
}

bool NestedMessage::encode(kiwi::ByteBuffer &bb) {
  if (_a) {
    bb.writeVarUint(1);
    bb.writeVarUint(*_a);
  }
  if (_b) {
    bb.writeVarUint(2);
    if (!(*_b).encode(bb)) return false;
  }
  if (_c) {
    bb.writeVarUint(3);
    bb.writeVarUint(*_c);
  }
  bb.writeVarUint(0);
  return true;
}

bool NestedMessage::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  while (true) {
    uint32_t type;
    if (!bb.readVarUint(type)) return false;
    switch (type) {
      case 0:
        return true;
      case 1:
        if (!bb.readVarUint(add_a(pool))) return false;
        break;
      case 2:
        if (!add_b(pool).decode(bb, pool)) return false;
        break;
      case 3:
        if (!bb.readVarUint(add_c(pool))) return false;
        break;
      default: return false;
    }
  }
}

bool BoolArrayStruct::encode(kiwi::ByteBuffer &bb) {
  if (!_x) return false;
  bb.writeVarUint(_x->size());
  for (bool &it : *_x) bb.writeByte(it);
  return true;
}

bool BoolArrayStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  if (!bb.readVarUint(count)) return false;
  for (bool &it : add_x(pool, count)) if (!bb.readByte(it)) return false;
  return true;
}

bool ByteArrayStruct::encode(kiwi::ByteBuffer &bb) {
  if (!_x) return false;
  bb.writeVarUint(_x->size());
  for (uint8_t &it : *_x) bb.writeByte(it);
  return true;
}

bool ByteArrayStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  if (!bb.readVarUint(count)) return false;
  for (uint8_t &it : add_x(pool, count)) if (!bb.readByte(it)) return false;
  return true;
}

bool IntArrayStruct::encode(kiwi::ByteBuffer &bb) {
  if (!_x) return false;
  bb.writeVarUint(_x->size());
  for (int32_t &it : *_x) bb.writeVarInt(it);
  return true;
}

bool IntArrayStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  if (!bb.readVarUint(count)) return false;
  for (int32_t &it : add_x(pool, count)) if (!bb.readVarInt(it)) return false;
  return true;
}

bool UintArrayStruct::encode(kiwi::ByteBuffer &bb) {
  if (!_x) return false;
  bb.writeVarUint(_x->size());
  for (uint32_t &it : *_x) bb.writeVarUint(it);
  return true;
}

bool UintArrayStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  if (!bb.readVarUint(count)) return false;
  for (uint32_t &it : add_x(pool, count)) if (!bb.readVarUint(it)) return false;
  return true;
}

bool FloatArrayStruct::encode(kiwi::ByteBuffer &bb) {
  if (!_x) return false;
  bb.writeVarUint(_x->size());
  for (float &it : *_x) bb.writeFloat(it);
  return true;
}

bool FloatArrayStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  if (!bb.readVarUint(count)) return false;
  for (float &it : add_x(pool, count)) if (!bb.readFloat(it)) return false;
  return true;
}

bool StringArrayStruct::encode(kiwi::ByteBuffer &bb) {
  if (!_x) return false;
  bb.writeVarUint(_x->size());
  for (kiwi::String &it : *_x) bb.writeString((it).c_str());
  return true;
}

bool StringArrayStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  if (!bb.readVarUint(count)) return false;
  for (kiwi::String &it : add_x(pool, count)) if (!bb.readString(it, pool)) return false;
  return true;
}

bool CompoundArrayStruct::encode(kiwi::ByteBuffer &bb) {
  if (!_x) return false;
  bb.writeVarUint(_x->size());
  for (uint32_t &it : *_x) bb.writeVarUint(it);
  if (!_y) return false;
  bb.writeVarUint(_y->size());
  for (uint32_t &it : *_y) bb.writeVarUint(it);
  return true;
}

bool CompoundArrayStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  if (!bb.readVarUint(count)) return false;
  for (uint32_t &it : add_x(pool, count)) if (!bb.readVarUint(it)) return false;
  if (!bb.readVarUint(count)) return false;
  for (uint32_t &it : add_y(pool, count)) if (!bb.readVarUint(it)) return false;
  return true;
}

bool BoolArrayMessage::encode(kiwi::ByteBuffer &bb) {
  if (_x) {
    bb.writeVarUint(1);
    bb.writeVarUint(_x->size());
    for (bool &it : *_x) bb.writeByte(it);
  }
  bb.writeVarUint(0);
  return true;
}

bool BoolArrayMessage::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  while (true) {
    uint32_t type;
    if (!bb.readVarUint(type)) return false;
    switch (type) {
      case 0:
        return true;
      case 1:
        if (!bb.readVarUint(count)) return false;
        for (bool &it : add_x(pool, count)) if (!bb.readByte(it)) return false;
        break;
      default: return false;
    }
  }
}

bool ByteArrayMessage::encode(kiwi::ByteBuffer &bb) {
  if (_x) {
    bb.writeVarUint(1);
    bb.writeVarUint(_x->size());
    for (uint8_t &it : *_x) bb.writeByte(it);
  }
  bb.writeVarUint(0);
  return true;
}

bool ByteArrayMessage::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  while (true) {
    uint32_t type;
    if (!bb.readVarUint(type)) return false;
    switch (type) {
      case 0:
        return true;
      case 1:
        if (!bb.readVarUint(count)) return false;
        for (uint8_t &it : add_x(pool, count)) if (!bb.readByte(it)) return false;
        break;
      default: return false;
    }
  }
}

bool IntArrayMessage::encode(kiwi::ByteBuffer &bb) {
  if (_x) {
    bb.writeVarUint(1);
    bb.writeVarUint(_x->size());
    for (int32_t &it : *_x) bb.writeVarInt(it);
  }
  bb.writeVarUint(0);
  return true;
}

bool IntArrayMessage::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  while (true) {
    uint32_t type;
    if (!bb.readVarUint(type)) return false;
    switch (type) {
      case 0:
        return true;
      case 1:
        if (!bb.readVarUint(count)) return false;
        for (int32_t &it : add_x(pool, count)) if (!bb.readVarInt(it)) return false;
        break;
      default: return false;
    }
  }
}

bool UintArrayMessage::encode(kiwi::ByteBuffer &bb) {
  if (_x) {
    bb.writeVarUint(1);
    bb.writeVarUint(_x->size());
    for (uint32_t &it : *_x) bb.writeVarUint(it);
  }
  bb.writeVarUint(0);
  return true;
}

bool UintArrayMessage::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  while (true) {
    uint32_t type;
    if (!bb.readVarUint(type)) return false;
    switch (type) {
      case 0:
        return true;
      case 1:
        if (!bb.readVarUint(count)) return false;
        for (uint32_t &it : add_x(pool, count)) if (!bb.readVarUint(it)) return false;
        break;
      default: return false;
    }
  }
}

bool FloatArrayMessage::encode(kiwi::ByteBuffer &bb) {
  if (_x) {
    bb.writeVarUint(1);
    bb.writeVarUint(_x->size());
    for (float &it : *_x) bb.writeFloat(it);
  }
  bb.writeVarUint(0);
  return true;
}

bool FloatArrayMessage::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  while (true) {
    uint32_t type;
    if (!bb.readVarUint(type)) return false;
    switch (type) {
      case 0:
        return true;
      case 1:
        if (!bb.readVarUint(count)) return false;
        for (float &it : add_x(pool, count)) if (!bb.readFloat(it)) return false;
        break;
      default: return false;
    }
  }
}

bool StringArrayMessage::encode(kiwi::ByteBuffer &bb) {
  if (_x) {
    bb.writeVarUint(1);
    bb.writeVarUint(_x->size());
    for (kiwi::String &it : *_x) bb.writeString((it).c_str());
  }
  bb.writeVarUint(0);
  return true;
}

bool StringArrayMessage::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  while (true) {
    uint32_t type;
    if (!bb.readVarUint(type)) return false;
    switch (type) {
      case 0:
        return true;
      case 1:
        if (!bb.readVarUint(count)) return false;
        for (kiwi::String &it : add_x(pool, count)) if (!bb.readString(it, pool)) return false;
        break;
      default: return false;
    }
  }
}

bool CompoundArrayMessage::encode(kiwi::ByteBuffer &bb) {
  if (_x) {
    bb.writeVarUint(1);
    bb.writeVarUint(_x->size());
    for (uint32_t &it : *_x) bb.writeVarUint(it);
  }
  if (_y) {
    bb.writeVarUint(2);
    bb.writeVarUint(_y->size());
    for (uint32_t &it : *_y) bb.writeVarUint(it);
  }
  bb.writeVarUint(0);
  return true;
}

bool CompoundArrayMessage::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  while (true) {
    uint32_t type;
    if (!bb.readVarUint(type)) return false;
    switch (type) {
      case 0:
        return true;
      case 1:
        if (!bb.readVarUint(count)) return false;
        for (uint32_t &it : add_x(pool, count)) if (!bb.readVarUint(it)) return false;
        break;
      case 2:
        if (!bb.readVarUint(count)) return false;
        for (uint32_t &it : add_y(pool, count)) if (!bb.readVarUint(it)) return false;
        break;
      default: return false;
    }
  }
}

bool RequiredField::encode(kiwi::ByteBuffer &bb) {
  if (!_x) return false;
  bb.writeVarUint(1);
  bb.writeVarInt(*_x);
  bb.writeVarUint(0);
  return true;
}

bool RequiredField::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  while (true) {
    uint32_t type;
    if (!bb.readVarUint(type)) return false;
    switch (type) {
      case 0:
        if (!_x) return false;
        return true;
      case 1:
        if (!bb.readVarInt(add_x(pool))) return false;
        break;
      default: return false;
    }
  }
}

}
