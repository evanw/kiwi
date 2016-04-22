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
class RecursiveMessage;
class RequiredField;

class BoolStruct {
public:
  bool *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  void set_x(const bool &value) { _flags[0] |= 1; _data_x = value; }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  bool _data_x = {};
};

class ByteStruct {
public:
  uint8_t *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  void set_x(const uint8_t &value) { _flags[0] |= 1; _data_x = value; }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  uint8_t _data_x = {};
};

class IntStruct {
public:
  int32_t *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  void set_x(const int32_t &value) { _flags[0] |= 1; _data_x = value; }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  int32_t _data_x = {};
};

class UintStruct {
public:
  uint32_t *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  void set_x(const uint32_t &value) { _flags[0] |= 1; _data_x = value; }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  uint32_t _data_x = {};
};

class FloatStruct {
public:
  float *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  void set_x(const float &value) { _flags[0] |= 1; _data_x = value; }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  float _data_x = {};
};

class StringStruct {
public:
  kiwi::String *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  void set_x(const kiwi::String &value) { _flags[0] |= 1; _data_x = value; }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::String _data_x = {};
};

class CompoundStruct {
public:
  uint32_t *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  void set_x(const uint32_t &value) { _flags[0] |= 1; _data_x = value; }

  uint32_t *y() { return _flags[0] & 2 ? &_data_y : nullptr; }
  void set_y(const uint32_t &value) { _flags[0] |= 2; _data_y = value; }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  uint32_t _data_x = {};
  uint32_t _data_y = {};
};

class NestedStruct {
public:
  uint32_t *a() { return _flags[0] & 1 ? &_data_a : nullptr; }
  void set_a(const uint32_t &value) { _flags[0] |= 1; _data_a = value; }

  CompoundStruct *b() { return _data_b; }
  void set_b(CompoundStruct *value) { _data_b = value; }

  uint32_t *c() { return _flags[0] & 4 ? &_data_c : nullptr; }
  void set_c(const uint32_t &value) { _flags[0] |= 4; _data_c = value; }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  uint32_t _data_a = {};
  CompoundStruct *_data_b = {};
  uint32_t _data_c = {};
};

class BoolMessage {
public:
  bool *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  void set_x(const bool &value) { _flags[0] |= 1; _data_x = value; }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  bool _data_x = {};
};

class ByteMessage {
public:
  uint8_t *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  void set_x(const uint8_t &value) { _flags[0] |= 1; _data_x = value; }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  uint8_t _data_x = {};
};

class IntMessage {
public:
  int32_t *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  void set_x(const int32_t &value) { _flags[0] |= 1; _data_x = value; }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  int32_t _data_x = {};
};

class UintMessage {
public:
  uint32_t *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  void set_x(const uint32_t &value) { _flags[0] |= 1; _data_x = value; }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  uint32_t _data_x = {};
};

class FloatMessage {
public:
  float *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  void set_x(const float &value) { _flags[0] |= 1; _data_x = value; }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  float _data_x = {};
};

class StringMessage {
public:
  kiwi::String *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  void set_x(const kiwi::String &value) { _flags[0] |= 1; _data_x = value; }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::String _data_x = {};
};

class CompoundMessage {
public:
  uint32_t *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  void set_x(const uint32_t &value) { _flags[0] |= 1; _data_x = value; }

  uint32_t *y() { return _flags[0] & 2 ? &_data_y : nullptr; }
  void set_y(const uint32_t &value) { _flags[0] |= 2; _data_y = value; }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  uint32_t _data_x = {};
  uint32_t _data_y = {};
};

class NestedMessage {
public:
  uint32_t *a() { return _flags[0] & 1 ? &_data_a : nullptr; }
  void set_a(const uint32_t &value) { _flags[0] |= 1; _data_a = value; }

  CompoundMessage *b() { return _data_b; }
  void set_b(CompoundMessage *value) { _data_b = value; }

  uint32_t *c() { return _flags[0] & 4 ? &_data_c : nullptr; }
  void set_c(const uint32_t &value) { _flags[0] |= 4; _data_c = value; }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  uint32_t _data_a = {};
  CompoundMessage *_data_b = {};
  uint32_t _data_c = {};
};

class BoolArrayStruct {
public:
  kiwi::Array<bool> *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  kiwi::Array<bool> &set_x(kiwi::MemoryPool &pool, uint32_t count) { _flags[0] |= 1; return _data_x = pool.array<bool>(count); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<bool> _data_x = {};
};

class ByteArrayStruct {
public:
  kiwi::Array<uint8_t> *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  kiwi::Array<uint8_t> &set_x(kiwi::MemoryPool &pool, uint32_t count) { _flags[0] |= 1; return _data_x = pool.array<uint8_t>(count); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint8_t> _data_x = {};
};

class IntArrayStruct {
public:
  kiwi::Array<int32_t> *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  kiwi::Array<int32_t> &set_x(kiwi::MemoryPool &pool, uint32_t count) { _flags[0] |= 1; return _data_x = pool.array<int32_t>(count); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<int32_t> _data_x = {};
};

class UintArrayStruct {
public:
  kiwi::Array<uint32_t> *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  kiwi::Array<uint32_t> &set_x(kiwi::MemoryPool &pool, uint32_t count) { _flags[0] |= 1; return _data_x = pool.array<uint32_t>(count); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint32_t> _data_x = {};
};

class FloatArrayStruct {
public:
  kiwi::Array<float> *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  kiwi::Array<float> &set_x(kiwi::MemoryPool &pool, uint32_t count) { _flags[0] |= 1; return _data_x = pool.array<float>(count); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<float> _data_x = {};
};

class StringArrayStruct {
public:
  kiwi::Array<kiwi::String> *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  kiwi::Array<kiwi::String> &set_x(kiwi::MemoryPool &pool, uint32_t count) { _flags[0] |= 1; return _data_x = pool.array<kiwi::String>(count); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<kiwi::String> _data_x = {};
};

class CompoundArrayStruct {
public:
  kiwi::Array<uint32_t> *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  kiwi::Array<uint32_t> &set_x(kiwi::MemoryPool &pool, uint32_t count) { _flags[0] |= 1; return _data_x = pool.array<uint32_t>(count); }

  kiwi::Array<uint32_t> *y() { return _flags[0] & 2 ? &_data_y : nullptr; }
  kiwi::Array<uint32_t> &set_y(kiwi::MemoryPool &pool, uint32_t count) { _flags[0] |= 2; return _data_y = pool.array<uint32_t>(count); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint32_t> _data_x = {};
  kiwi::Array<uint32_t> _data_y = {};
};

class BoolArrayMessage {
public:
  kiwi::Array<bool> *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  kiwi::Array<bool> &set_x(kiwi::MemoryPool &pool, uint32_t count) { _flags[0] |= 1; return _data_x = pool.array<bool>(count); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<bool> _data_x = {};
};

class ByteArrayMessage {
public:
  kiwi::Array<uint8_t> *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  kiwi::Array<uint8_t> &set_x(kiwi::MemoryPool &pool, uint32_t count) { _flags[0] |= 1; return _data_x = pool.array<uint8_t>(count); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint8_t> _data_x = {};
};

class IntArrayMessage {
public:
  kiwi::Array<int32_t> *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  kiwi::Array<int32_t> &set_x(kiwi::MemoryPool &pool, uint32_t count) { _flags[0] |= 1; return _data_x = pool.array<int32_t>(count); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<int32_t> _data_x = {};
};

class UintArrayMessage {
public:
  kiwi::Array<uint32_t> *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  kiwi::Array<uint32_t> &set_x(kiwi::MemoryPool &pool, uint32_t count) { _flags[0] |= 1; return _data_x = pool.array<uint32_t>(count); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint32_t> _data_x = {};
};

class FloatArrayMessage {
public:
  kiwi::Array<float> *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  kiwi::Array<float> &set_x(kiwi::MemoryPool &pool, uint32_t count) { _flags[0] |= 1; return _data_x = pool.array<float>(count); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<float> _data_x = {};
};

class StringArrayMessage {
public:
  kiwi::Array<kiwi::String> *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  kiwi::Array<kiwi::String> &set_x(kiwi::MemoryPool &pool, uint32_t count) { _flags[0] |= 1; return _data_x = pool.array<kiwi::String>(count); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<kiwi::String> _data_x = {};
};

class CompoundArrayMessage {
public:
  kiwi::Array<uint32_t> *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  kiwi::Array<uint32_t> &set_x(kiwi::MemoryPool &pool, uint32_t count) { _flags[0] |= 1; return _data_x = pool.array<uint32_t>(count); }

  kiwi::Array<uint32_t> *y() { return _flags[0] & 2 ? &_data_y : nullptr; }
  kiwi::Array<uint32_t> &set_y(kiwi::MemoryPool &pool, uint32_t count) { _flags[0] |= 2; return _data_y = pool.array<uint32_t>(count); }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint32_t> _data_x = {};
  kiwi::Array<uint32_t> _data_y = {};
};

class RecursiveMessage {
public:
  RecursiveMessage *x() { return _data_x; }
  void set_x(RecursiveMessage *value) { _data_x = value; }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  RecursiveMessage *_data_x = {};
};

class RequiredField {
public:
  int32_t *x() { return _flags[0] & 1 ? &_data_x : nullptr; }
  void set_x(const int32_t &value) { _flags[0] |= 1; _data_x = value; }

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  int32_t _data_x = {};
};

bool BoolStruct::encode(kiwi::ByteBuffer &bb) {
  if (x() == nullptr) return false;
  bb.writeByte(_data_x);
  return true;
}

bool BoolStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  if (!bb.readByte(_data_x)) return false;
  set_x(_data_x);
  return true;
}

bool ByteStruct::encode(kiwi::ByteBuffer &bb) {
  if (x() == nullptr) return false;
  bb.writeByte(_data_x);
  return true;
}

bool ByteStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  if (!bb.readByte(_data_x)) return false;
  set_x(_data_x);
  return true;
}

bool IntStruct::encode(kiwi::ByteBuffer &bb) {
  if (x() == nullptr) return false;
  bb.writeVarInt(_data_x);
  return true;
}

bool IntStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  if (!bb.readVarInt(_data_x)) return false;
  set_x(_data_x);
  return true;
}

bool UintStruct::encode(kiwi::ByteBuffer &bb) {
  if (x() == nullptr) return false;
  bb.writeVarUint(_data_x);
  return true;
}

bool UintStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  if (!bb.readVarUint(_data_x)) return false;
  set_x(_data_x);
  return true;
}

bool FloatStruct::encode(kiwi::ByteBuffer &bb) {
  if (x() == nullptr) return false;
  bb.writeFloat(_data_x);
  return true;
}

bool FloatStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  if (!bb.readFloat(_data_x)) return false;
  set_x(_data_x);
  return true;
}

bool StringStruct::encode(kiwi::ByteBuffer &bb) {
  if (x() == nullptr) return false;
  bb.writeString(_data_x.c_str());
  return true;
}

bool StringStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  if (!bb.readString(_data_x, pool)) return false;
  set_x(_data_x);
  return true;
}

bool CompoundStruct::encode(kiwi::ByteBuffer &bb) {
  if (x() == nullptr) return false;
  bb.writeVarUint(_data_x);
  if (y() == nullptr) return false;
  bb.writeVarUint(_data_y);
  return true;
}

bool CompoundStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  if (!bb.readVarUint(_data_x)) return false;
  set_x(_data_x);
  if (!bb.readVarUint(_data_y)) return false;
  set_y(_data_y);
  return true;
}

bool NestedStruct::encode(kiwi::ByteBuffer &bb) {
  if (a() == nullptr) return false;
  bb.writeVarUint(_data_a);
  if (b() == nullptr) return false;
  if (!_data_b->encode(bb)) return false;
  if (c() == nullptr) return false;
  bb.writeVarUint(_data_c);
  return true;
}

bool NestedStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  if (!bb.readVarUint(_data_a)) return false;
  set_a(_data_a);
  _data_b = pool.allocate<CompoundStruct>();
  if (!_data_b->decode(bb, pool)) return false;
  if (!bb.readVarUint(_data_c)) return false;
  set_c(_data_c);
  return true;
}

bool BoolMessage::encode(kiwi::ByteBuffer &bb) {
  if (x() != nullptr) {
    bb.writeVarUint(1);
    bb.writeByte(_data_x);
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
        if (!bb.readByte(_data_x)) return false;
        set_x(_data_x);
        break;
      default: return false;
    }
  }
}

bool ByteMessage::encode(kiwi::ByteBuffer &bb) {
  if (x() != nullptr) {
    bb.writeVarUint(1);
    bb.writeByte(_data_x);
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
        if (!bb.readByte(_data_x)) return false;
        set_x(_data_x);
        break;
      default: return false;
    }
  }
}

bool IntMessage::encode(kiwi::ByteBuffer &bb) {
  if (x() != nullptr) {
    bb.writeVarUint(1);
    bb.writeVarInt(_data_x);
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
        if (!bb.readVarInt(_data_x)) return false;
        set_x(_data_x);
        break;
      default: return false;
    }
  }
}

bool UintMessage::encode(kiwi::ByteBuffer &bb) {
  if (x() != nullptr) {
    bb.writeVarUint(1);
    bb.writeVarUint(_data_x);
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
        if (!bb.readVarUint(_data_x)) return false;
        set_x(_data_x);
        break;
      default: return false;
    }
  }
}

bool FloatMessage::encode(kiwi::ByteBuffer &bb) {
  if (x() != nullptr) {
    bb.writeVarUint(1);
    bb.writeFloat(_data_x);
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
        if (!bb.readFloat(_data_x)) return false;
        set_x(_data_x);
        break;
      default: return false;
    }
  }
}

bool StringMessage::encode(kiwi::ByteBuffer &bb) {
  if (x() != nullptr) {
    bb.writeVarUint(1);
    bb.writeString(_data_x.c_str());
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
        if (!bb.readString(_data_x, pool)) return false;
        set_x(_data_x);
        break;
      default: return false;
    }
  }
}

bool CompoundMessage::encode(kiwi::ByteBuffer &bb) {
  if (x() != nullptr) {
    bb.writeVarUint(1);
    bb.writeVarUint(_data_x);
  }
  if (y() != nullptr) {
    bb.writeVarUint(2);
    bb.writeVarUint(_data_y);
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
        if (!bb.readVarUint(_data_x)) return false;
        set_x(_data_x);
        break;
      case 2:
        if (!bb.readVarUint(_data_y)) return false;
        set_y(_data_y);
        break;
      default: return false;
    }
  }
}

bool NestedMessage::encode(kiwi::ByteBuffer &bb) {
  if (a() != nullptr) {
    bb.writeVarUint(1);
    bb.writeVarUint(_data_a);
  }
  if (b() != nullptr) {
    bb.writeVarUint(2);
    if (!_data_b->encode(bb)) return false;
  }
  if (c() != nullptr) {
    bb.writeVarUint(3);
    bb.writeVarUint(_data_c);
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
        if (!bb.readVarUint(_data_a)) return false;
        set_a(_data_a);
        break;
      case 2:
        _data_b = pool.allocate<CompoundMessage>();
        if (!_data_b->decode(bb, pool)) return false;
        break;
      case 3:
        if (!bb.readVarUint(_data_c)) return false;
        set_c(_data_c);
        break;
      default: return false;
    }
  }
}

bool BoolArrayStruct::encode(kiwi::ByteBuffer &bb) {
  if (x() == nullptr) return false;
  bb.writeVarUint(_data_x.size());
  for (bool &it : _data_x) bb.writeByte(it);
  return true;
}

bool BoolArrayStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  if (!bb.readVarUint(count)) return false;
  for (bool &it : set_x(pool, count)) if (!bb.readByte(it)) return false;
  return true;
}

bool ByteArrayStruct::encode(kiwi::ByteBuffer &bb) {
  if (x() == nullptr) return false;
  bb.writeVarUint(_data_x.size());
  for (uint8_t &it : _data_x) bb.writeByte(it);
  return true;
}

bool ByteArrayStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  if (!bb.readVarUint(count)) return false;
  for (uint8_t &it : set_x(pool, count)) if (!bb.readByte(it)) return false;
  return true;
}

bool IntArrayStruct::encode(kiwi::ByteBuffer &bb) {
  if (x() == nullptr) return false;
  bb.writeVarUint(_data_x.size());
  for (int32_t &it : _data_x) bb.writeVarInt(it);
  return true;
}

bool IntArrayStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  if (!bb.readVarUint(count)) return false;
  for (int32_t &it : set_x(pool, count)) if (!bb.readVarInt(it)) return false;
  return true;
}

bool UintArrayStruct::encode(kiwi::ByteBuffer &bb) {
  if (x() == nullptr) return false;
  bb.writeVarUint(_data_x.size());
  for (uint32_t &it : _data_x) bb.writeVarUint(it);
  return true;
}

bool UintArrayStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  if (!bb.readVarUint(count)) return false;
  for (uint32_t &it : set_x(pool, count)) if (!bb.readVarUint(it)) return false;
  return true;
}

bool FloatArrayStruct::encode(kiwi::ByteBuffer &bb) {
  if (x() == nullptr) return false;
  bb.writeVarUint(_data_x.size());
  for (float &it : _data_x) bb.writeFloat(it);
  return true;
}

bool FloatArrayStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  if (!bb.readVarUint(count)) return false;
  for (float &it : set_x(pool, count)) if (!bb.readFloat(it)) return false;
  return true;
}

bool StringArrayStruct::encode(kiwi::ByteBuffer &bb) {
  if (x() == nullptr) return false;
  bb.writeVarUint(_data_x.size());
  for (kiwi::String &it : _data_x) bb.writeString(it.c_str());
  return true;
}

bool StringArrayStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  if (!bb.readVarUint(count)) return false;
  for (kiwi::String &it : set_x(pool, count)) if (!bb.readString(it, pool)) return false;
  return true;
}

bool CompoundArrayStruct::encode(kiwi::ByteBuffer &bb) {
  if (x() == nullptr) return false;
  bb.writeVarUint(_data_x.size());
  for (uint32_t &it : _data_x) bb.writeVarUint(it);
  if (y() == nullptr) return false;
  bb.writeVarUint(_data_y.size());
  for (uint32_t &it : _data_y) bb.writeVarUint(it);
  return true;
}

bool CompoundArrayStruct::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  uint32_t count;
  if (!bb.readVarUint(count)) return false;
  for (uint32_t &it : set_x(pool, count)) if (!bb.readVarUint(it)) return false;
  if (!bb.readVarUint(count)) return false;
  for (uint32_t &it : set_y(pool, count)) if (!bb.readVarUint(it)) return false;
  return true;
}

bool BoolArrayMessage::encode(kiwi::ByteBuffer &bb) {
  if (x() != nullptr) {
    bb.writeVarUint(1);
    bb.writeVarUint(_data_x.size());
    for (bool &it : _data_x) bb.writeByte(it);
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
        for (bool &it : set_x(pool, count)) if (!bb.readByte(it)) return false;
        break;
      default: return false;
    }
  }
}

bool ByteArrayMessage::encode(kiwi::ByteBuffer &bb) {
  if (x() != nullptr) {
    bb.writeVarUint(1);
    bb.writeVarUint(_data_x.size());
    for (uint8_t &it : _data_x) bb.writeByte(it);
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
        for (uint8_t &it : set_x(pool, count)) if (!bb.readByte(it)) return false;
        break;
      default: return false;
    }
  }
}

bool IntArrayMessage::encode(kiwi::ByteBuffer &bb) {
  if (x() != nullptr) {
    bb.writeVarUint(1);
    bb.writeVarUint(_data_x.size());
    for (int32_t &it : _data_x) bb.writeVarInt(it);
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
        for (int32_t &it : set_x(pool, count)) if (!bb.readVarInt(it)) return false;
        break;
      default: return false;
    }
  }
}

bool UintArrayMessage::encode(kiwi::ByteBuffer &bb) {
  if (x() != nullptr) {
    bb.writeVarUint(1);
    bb.writeVarUint(_data_x.size());
    for (uint32_t &it : _data_x) bb.writeVarUint(it);
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
        for (uint32_t &it : set_x(pool, count)) if (!bb.readVarUint(it)) return false;
        break;
      default: return false;
    }
  }
}

bool FloatArrayMessage::encode(kiwi::ByteBuffer &bb) {
  if (x() != nullptr) {
    bb.writeVarUint(1);
    bb.writeVarUint(_data_x.size());
    for (float &it : _data_x) bb.writeFloat(it);
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
        for (float &it : set_x(pool, count)) if (!bb.readFloat(it)) return false;
        break;
      default: return false;
    }
  }
}

bool StringArrayMessage::encode(kiwi::ByteBuffer &bb) {
  if (x() != nullptr) {
    bb.writeVarUint(1);
    bb.writeVarUint(_data_x.size());
    for (kiwi::String &it : _data_x) bb.writeString(it.c_str());
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
        for (kiwi::String &it : set_x(pool, count)) if (!bb.readString(it, pool)) return false;
        break;
      default: return false;
    }
  }
}

bool CompoundArrayMessage::encode(kiwi::ByteBuffer &bb) {
  if (x() != nullptr) {
    bb.writeVarUint(1);
    bb.writeVarUint(_data_x.size());
    for (uint32_t &it : _data_x) bb.writeVarUint(it);
  }
  if (y() != nullptr) {
    bb.writeVarUint(2);
    bb.writeVarUint(_data_y.size());
    for (uint32_t &it : _data_y) bb.writeVarUint(it);
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
        for (uint32_t &it : set_x(pool, count)) if (!bb.readVarUint(it)) return false;
        break;
      case 2:
        if (!bb.readVarUint(count)) return false;
        for (uint32_t &it : set_y(pool, count)) if (!bb.readVarUint(it)) return false;
        break;
      default: return false;
    }
  }
}

bool RecursiveMessage::encode(kiwi::ByteBuffer &bb) {
  if (x() != nullptr) {
    bb.writeVarUint(1);
    if (!_data_x->encode(bb)) return false;
  }
  bb.writeVarUint(0);
  return true;
}

bool RecursiveMessage::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  while (true) {
    uint32_t type;
    if (!bb.readVarUint(type)) return false;
    switch (type) {
      case 0:
        return true;
      case 1:
        _data_x = pool.allocate<RecursiveMessage>();
        if (!_data_x->decode(bb, pool)) return false;
        break;
      default: return false;
    }
  }
}

bool RequiredField::encode(kiwi::ByteBuffer &bb) {
  if (x() == nullptr) return false;
  bb.writeVarUint(1);
  bb.writeVarInt(_data_x);
  bb.writeVarUint(0);
  return true;
}

bool RequiredField::decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool) {
  while (true) {
    uint32_t type;
    if (!bb.readVarUint(type)) return false;
    switch (type) {
      case 0:
        if (x() == nullptr) return false;
        return true;
      case 1:
        if (!bb.readVarInt(_data_x)) return false;
        set_x(_data_x);
        break;
      default: return false;
    }
  }
}

}
