#include "kiwi.h"

namespace test {

enum class Enum : uint32_t {
  A = 100,
  B = 200,
};

class EnumStruct;
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

class EnumStruct {
public:
  Enum *x();
  const Enum *x() const;
  void set_x(const Enum &value);

  kiwi::Array<Enum> *y();
  const kiwi::Array<Enum> *y() const;
  kiwi::Array<Enum> &set_y(kiwi::MemoryPool &pool, uint32_t count);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  Enum _data_x = {};
  kiwi::Array<Enum> _data_y = {};
};

class BoolStruct {
public:
  bool *x();
  const bool *x() const;
  void set_x(const bool &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  bool _data_x = {};
};

class ByteStruct {
public:
  uint8_t *x();
  const uint8_t *x() const;
  void set_x(const uint8_t &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  uint8_t _data_x = {};
};

class IntStruct {
public:
  int32_t *x();
  const int32_t *x() const;
  void set_x(const int32_t &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  int32_t _data_x = {};
};

class UintStruct {
public:
  uint32_t *x();
  const uint32_t *x() const;
  void set_x(const uint32_t &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  uint32_t _data_x = {};
};

class FloatStruct {
public:
  float *x();
  const float *x() const;
  void set_x(const float &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  float _data_x = {};
};

class StringStruct {
public:
  kiwi::String *x();
  const kiwi::String *x() const;
  void set_x(const kiwi::String &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::String _data_x = {};
};

class CompoundStruct {
public:
  uint32_t *x();
  const uint32_t *x() const;
  void set_x(const uint32_t &value);

  uint32_t *y();
  const uint32_t *y() const;
  void set_y(const uint32_t &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  uint32_t _data_x = {};
  uint32_t _data_y = {};
};

class NestedStruct {
public:
  uint32_t *a();
  const uint32_t *a() const;
  void set_a(const uint32_t &value);

  CompoundStruct *b();
  const CompoundStruct *b() const;
  void set_b(CompoundStruct *value);

  uint32_t *c();
  const uint32_t *c() const;
  void set_c(const uint32_t &value);

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
  bool *x();
  const bool *x() const;
  void set_x(const bool &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  bool _data_x = {};
};

class ByteMessage {
public:
  uint8_t *x();
  const uint8_t *x() const;
  void set_x(const uint8_t &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  uint8_t _data_x = {};
};

class IntMessage {
public:
  int32_t *x();
  const int32_t *x() const;
  void set_x(const int32_t &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  int32_t _data_x = {};
};

class UintMessage {
public:
  uint32_t *x();
  const uint32_t *x() const;
  void set_x(const uint32_t &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  uint32_t _data_x = {};
};

class FloatMessage {
public:
  float *x();
  const float *x() const;
  void set_x(const float &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  float _data_x = {};
};

class StringMessage {
public:
  kiwi::String *x();
  const kiwi::String *x() const;
  void set_x(const kiwi::String &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::String _data_x = {};
};

class CompoundMessage {
public:
  uint32_t *x();
  const uint32_t *x() const;
  void set_x(const uint32_t &value);

  uint32_t *y();
  const uint32_t *y() const;
  void set_y(const uint32_t &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  uint32_t _data_x = {};
  uint32_t _data_y = {};
};

class NestedMessage {
public:
  uint32_t *a();
  const uint32_t *a() const;
  void set_a(const uint32_t &value);

  CompoundMessage *b();
  const CompoundMessage *b() const;
  void set_b(CompoundMessage *value);

  uint32_t *c();
  const uint32_t *c() const;
  void set_c(const uint32_t &value);

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
  kiwi::Array<bool> *x();
  const kiwi::Array<bool> *x() const;
  kiwi::Array<bool> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<bool> _data_x = {};
};

class ByteArrayStruct {
public:
  kiwi::Array<uint8_t> *x();
  const kiwi::Array<uint8_t> *x() const;
  kiwi::Array<uint8_t> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint8_t> _data_x = {};
};

class IntArrayStruct {
public:
  kiwi::Array<int32_t> *x();
  const kiwi::Array<int32_t> *x() const;
  kiwi::Array<int32_t> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<int32_t> _data_x = {};
};

class UintArrayStruct {
public:
  kiwi::Array<uint32_t> *x();
  const kiwi::Array<uint32_t> *x() const;
  kiwi::Array<uint32_t> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint32_t> _data_x = {};
};

class FloatArrayStruct {
public:
  kiwi::Array<float> *x();
  const kiwi::Array<float> *x() const;
  kiwi::Array<float> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<float> _data_x = {};
};

class StringArrayStruct {
public:
  kiwi::Array<kiwi::String> *x();
  const kiwi::Array<kiwi::String> *x() const;
  kiwi::Array<kiwi::String> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<kiwi::String> _data_x = {};
};

class CompoundArrayStruct {
public:
  kiwi::Array<uint32_t> *x();
  const kiwi::Array<uint32_t> *x() const;
  kiwi::Array<uint32_t> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  kiwi::Array<uint32_t> *y();
  const kiwi::Array<uint32_t> *y() const;
  kiwi::Array<uint32_t> &set_y(kiwi::MemoryPool &pool, uint32_t count);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint32_t> _data_x = {};
  kiwi::Array<uint32_t> _data_y = {};
};

class BoolArrayMessage {
public:
  kiwi::Array<bool> *x();
  const kiwi::Array<bool> *x() const;
  kiwi::Array<bool> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<bool> _data_x = {};
};

class ByteArrayMessage {
public:
  kiwi::Array<uint8_t> *x();
  const kiwi::Array<uint8_t> *x() const;
  kiwi::Array<uint8_t> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint8_t> _data_x = {};
};

class IntArrayMessage {
public:
  kiwi::Array<int32_t> *x();
  const kiwi::Array<int32_t> *x() const;
  kiwi::Array<int32_t> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<int32_t> _data_x = {};
};

class UintArrayMessage {
public:
  kiwi::Array<uint32_t> *x();
  const kiwi::Array<uint32_t> *x() const;
  kiwi::Array<uint32_t> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint32_t> _data_x = {};
};

class FloatArrayMessage {
public:
  kiwi::Array<float> *x();
  const kiwi::Array<float> *x() const;
  kiwi::Array<float> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<float> _data_x = {};
};

class StringArrayMessage {
public:
  kiwi::Array<kiwi::String> *x();
  const kiwi::Array<kiwi::String> *x() const;
  kiwi::Array<kiwi::String> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<kiwi::String> _data_x = {};
};

class CompoundArrayMessage {
public:
  kiwi::Array<uint32_t> *x();
  const kiwi::Array<uint32_t> *x() const;
  kiwi::Array<uint32_t> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  kiwi::Array<uint32_t> *y();
  const kiwi::Array<uint32_t> *y() const;
  kiwi::Array<uint32_t> &set_y(kiwi::MemoryPool &pool, uint32_t count);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint32_t> _data_x = {};
  kiwi::Array<uint32_t> _data_y = {};
};

class RecursiveMessage {
public:
  RecursiveMessage *x();
  const RecursiveMessage *x() const;
  void set_x(RecursiveMessage *value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  RecursiveMessage *_data_x = {};
};

class RequiredField {
public:
  int32_t *x();
  const int32_t *x() const;
  void set_x(const int32_t &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool);

private:
  uint32_t _flags[1] = {};
  int32_t _data_x = {};
};

#ifdef IMPLEMENT_SCHEMA_H

Enum *EnumStruct::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const Enum *EnumStruct::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

void EnumStruct::set_x(const Enum &value) {
  _flags[0] |= 1; _data_x = value;
}

kiwi::Array<Enum> *EnumStruct::y() {
  return _flags[0] & 2 ? &_data_y : nullptr;
}

const kiwi::Array<Enum> *EnumStruct::y() const {
  return _flags[0] & 2 ? &_data_y : nullptr;
}

kiwi::Array<Enum> &EnumStruct::set_y(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 2; return _data_y = pool.array<Enum>(count);
}

bool EnumStruct::encode(kiwi::ByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(static_cast<uint32_t>(_data_x));
  if (y() == nullptr) return false;
  _bb.writeVarUint(_data_y.size());
  for (Enum &_it : _data_y) _bb.writeVarUint(static_cast<uint32_t>(_it));
  return true;
}

bool EnumStruct::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  uint32_t _count;
  if (!_bb.readVarUint(reinterpret_cast<uint32_t &>(_data_x))) return false;
  set_x(_data_x);
  if (!_bb.readVarUint(_count)) return false;
  for (Enum &_it : set_y(_pool, _count)) if (!_bb.readVarUint(reinterpret_cast<uint32_t &>(_it))) return false;
  return true;
}

bool *BoolStruct::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const bool *BoolStruct::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

void BoolStruct::set_x(const bool &value) {
  _flags[0] |= 1; _data_x = value;
}

bool BoolStruct::encode(kiwi::ByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeByte(_data_x);
  return true;
}

bool BoolStruct::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  if (!_bb.readByte(_data_x)) return false;
  set_x(_data_x);
  return true;
}

uint8_t *ByteStruct::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const uint8_t *ByteStruct::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

void ByteStruct::set_x(const uint8_t &value) {
  _flags[0] |= 1; _data_x = value;
}

bool ByteStruct::encode(kiwi::ByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeByte(_data_x);
  return true;
}

bool ByteStruct::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  if (!_bb.readByte(_data_x)) return false;
  set_x(_data_x);
  return true;
}

int32_t *IntStruct::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const int32_t *IntStruct::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

void IntStruct::set_x(const int32_t &value) {
  _flags[0] |= 1; _data_x = value;
}

bool IntStruct::encode(kiwi::ByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarInt(_data_x);
  return true;
}

bool IntStruct::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  if (!_bb.readVarInt(_data_x)) return false;
  set_x(_data_x);
  return true;
}

uint32_t *UintStruct::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const uint32_t *UintStruct::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

void UintStruct::set_x(const uint32_t &value) {
  _flags[0] |= 1; _data_x = value;
}

bool UintStruct::encode(kiwi::ByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(_data_x);
  return true;
}

bool UintStruct::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  if (!_bb.readVarUint(_data_x)) return false;
  set_x(_data_x);
  return true;
}

float *FloatStruct::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const float *FloatStruct::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

void FloatStruct::set_x(const float &value) {
  _flags[0] |= 1; _data_x = value;
}

bool FloatStruct::encode(kiwi::ByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarFloat(_data_x);
  return true;
}

bool FloatStruct::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  if (!_bb.readVarFloat(_data_x)) return false;
  set_x(_data_x);
  return true;
}

kiwi::String *StringStruct::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const kiwi::String *StringStruct::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

void StringStruct::set_x(const kiwi::String &value) {
  _flags[0] |= 1; _data_x = value;
}

bool StringStruct::encode(kiwi::ByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeString(_data_x.c_str());
  return true;
}

bool StringStruct::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  if (!_bb.readString(_data_x, _pool)) return false;
  set_x(_data_x);
  return true;
}

uint32_t *CompoundStruct::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const uint32_t *CompoundStruct::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

void CompoundStruct::set_x(const uint32_t &value) {
  _flags[0] |= 1; _data_x = value;
}

uint32_t *CompoundStruct::y() {
  return _flags[0] & 2 ? &_data_y : nullptr;
}

const uint32_t *CompoundStruct::y() const {
  return _flags[0] & 2 ? &_data_y : nullptr;
}

void CompoundStruct::set_y(const uint32_t &value) {
  _flags[0] |= 2; _data_y = value;
}

bool CompoundStruct::encode(kiwi::ByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(_data_x);
  if (y() == nullptr) return false;
  _bb.writeVarUint(_data_y);
  return true;
}

bool CompoundStruct::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  if (!_bb.readVarUint(_data_x)) return false;
  set_x(_data_x);
  if (!_bb.readVarUint(_data_y)) return false;
  set_y(_data_y);
  return true;
}

uint32_t *NestedStruct::a() {
  return _flags[0] & 1 ? &_data_a : nullptr;
}

const uint32_t *NestedStruct::a() const {
  return _flags[0] & 1 ? &_data_a : nullptr;
}

void NestedStruct::set_a(const uint32_t &value) {
  _flags[0] |= 1; _data_a = value;
}

CompoundStruct *NestedStruct::b() {
  return _data_b;
}

const CompoundStruct *NestedStruct::b() const {
  return _data_b;
}

void NestedStruct::set_b(CompoundStruct *value) {
  _data_b = value;
}

uint32_t *NestedStruct::c() {
  return _flags[0] & 4 ? &_data_c : nullptr;
}

const uint32_t *NestedStruct::c() const {
  return _flags[0] & 4 ? &_data_c : nullptr;
}

void NestedStruct::set_c(const uint32_t &value) {
  _flags[0] |= 4; _data_c = value;
}

bool NestedStruct::encode(kiwi::ByteBuffer &_bb) {
  if (a() == nullptr) return false;
  _bb.writeVarUint(_data_a);
  if (b() == nullptr) return false;
  if (!_data_b->encode(_bb)) return false;
  if (c() == nullptr) return false;
  _bb.writeVarUint(_data_c);
  return true;
}

bool NestedStruct::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  if (!_bb.readVarUint(_data_a)) return false;
  set_a(_data_a);
  _data_b = _pool.allocate<CompoundStruct>();
  if (!_data_b->decode(_bb, _pool)) return false;
  if (!_bb.readVarUint(_data_c)) return false;
  set_c(_data_c);
  return true;
}

bool *BoolMessage::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const bool *BoolMessage::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

void BoolMessage::set_x(const bool &value) {
  _flags[0] |= 1; _data_x = value;
}

bool BoolMessage::encode(kiwi::ByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeByte(_data_x);
  }
  _bb.writeVarUint(0);
  return true;
}

bool BoolMessage::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1:
        if (!_bb.readByte(_data_x)) return false;
        set_x(_data_x);
        break;
      default: return false;
    }
  }
}

uint8_t *ByteMessage::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const uint8_t *ByteMessage::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

void ByteMessage::set_x(const uint8_t &value) {
  _flags[0] |= 1; _data_x = value;
}

bool ByteMessage::encode(kiwi::ByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeByte(_data_x);
  }
  _bb.writeVarUint(0);
  return true;
}

bool ByteMessage::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1:
        if (!_bb.readByte(_data_x)) return false;
        set_x(_data_x);
        break;
      default: return false;
    }
  }
}

int32_t *IntMessage::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const int32_t *IntMessage::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

void IntMessage::set_x(const int32_t &value) {
  _flags[0] |= 1; _data_x = value;
}

bool IntMessage::encode(kiwi::ByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarInt(_data_x);
  }
  _bb.writeVarUint(0);
  return true;
}

bool IntMessage::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1:
        if (!_bb.readVarInt(_data_x)) return false;
        set_x(_data_x);
        break;
      default: return false;
    }
  }
}

uint32_t *UintMessage::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const uint32_t *UintMessage::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

void UintMessage::set_x(const uint32_t &value) {
  _flags[0] |= 1; _data_x = value;
}

bool UintMessage::encode(kiwi::ByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_x);
  }
  _bb.writeVarUint(0);
  return true;
}

bool UintMessage::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1:
        if (!_bb.readVarUint(_data_x)) return false;
        set_x(_data_x);
        break;
      default: return false;
    }
  }
}

float *FloatMessage::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const float *FloatMessage::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

void FloatMessage::set_x(const float &value) {
  _flags[0] |= 1; _data_x = value;
}

bool FloatMessage::encode(kiwi::ByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarFloat(_data_x);
  }
  _bb.writeVarUint(0);
  return true;
}

bool FloatMessage::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1:
        if (!_bb.readVarFloat(_data_x)) return false;
        set_x(_data_x);
        break;
      default: return false;
    }
  }
}

kiwi::String *StringMessage::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const kiwi::String *StringMessage::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

void StringMessage::set_x(const kiwi::String &value) {
  _flags[0] |= 1; _data_x = value;
}

bool StringMessage::encode(kiwi::ByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeString(_data_x.c_str());
  }
  _bb.writeVarUint(0);
  return true;
}

bool StringMessage::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1:
        if (!_bb.readString(_data_x, _pool)) return false;
        set_x(_data_x);
        break;
      default: return false;
    }
  }
}

uint32_t *CompoundMessage::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const uint32_t *CompoundMessage::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

void CompoundMessage::set_x(const uint32_t &value) {
  _flags[0] |= 1; _data_x = value;
}

uint32_t *CompoundMessage::y() {
  return _flags[0] & 2 ? &_data_y : nullptr;
}

const uint32_t *CompoundMessage::y() const {
  return _flags[0] & 2 ? &_data_y : nullptr;
}

void CompoundMessage::set_y(const uint32_t &value) {
  _flags[0] |= 2; _data_y = value;
}

bool CompoundMessage::encode(kiwi::ByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_x);
  }
  if (y() != nullptr) {
    _bb.writeVarUint(2);
    _bb.writeVarUint(_data_y);
  }
  _bb.writeVarUint(0);
  return true;
}

bool CompoundMessage::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1:
        if (!_bb.readVarUint(_data_x)) return false;
        set_x(_data_x);
        break;
      case 2:
        if (!_bb.readVarUint(_data_y)) return false;
        set_y(_data_y);
        break;
      default: return false;
    }
  }
}

uint32_t *NestedMessage::a() {
  return _flags[0] & 1 ? &_data_a : nullptr;
}

const uint32_t *NestedMessage::a() const {
  return _flags[0] & 1 ? &_data_a : nullptr;
}

void NestedMessage::set_a(const uint32_t &value) {
  _flags[0] |= 1; _data_a = value;
}

CompoundMessage *NestedMessage::b() {
  return _data_b;
}

const CompoundMessage *NestedMessage::b() const {
  return _data_b;
}

void NestedMessage::set_b(CompoundMessage *value) {
  _data_b = value;
}

uint32_t *NestedMessage::c() {
  return _flags[0] & 4 ? &_data_c : nullptr;
}

const uint32_t *NestedMessage::c() const {
  return _flags[0] & 4 ? &_data_c : nullptr;
}

void NestedMessage::set_c(const uint32_t &value) {
  _flags[0] |= 4; _data_c = value;
}

bool NestedMessage::encode(kiwi::ByteBuffer &_bb) {
  if (a() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_a);
  }
  if (b() != nullptr) {
    _bb.writeVarUint(2);
    if (!_data_b->encode(_bb)) return false;
  }
  if (c() != nullptr) {
    _bb.writeVarUint(3);
    _bb.writeVarUint(_data_c);
  }
  _bb.writeVarUint(0);
  return true;
}

bool NestedMessage::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1:
        if (!_bb.readVarUint(_data_a)) return false;
        set_a(_data_a);
        break;
      case 2:
        _data_b = _pool.allocate<CompoundMessage>();
        if (!_data_b->decode(_bb, _pool)) return false;
        break;
      case 3:
        if (!_bb.readVarUint(_data_c)) return false;
        set_c(_data_c);
        break;
      default: return false;
    }
  }
}

kiwi::Array<bool> *BoolArrayStruct::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const kiwi::Array<bool> *BoolArrayStruct::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

kiwi::Array<bool> &BoolArrayStruct::set_x(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 1; return _data_x = pool.array<bool>(count);
}

bool BoolArrayStruct::encode(kiwi::ByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(_data_x.size());
  for (bool &_it : _data_x) _bb.writeByte(_it);
  return true;
}

bool BoolArrayStruct::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  uint32_t _count;
  if (!_bb.readVarUint(_count)) return false;
  for (bool &_it : set_x(_pool, _count)) if (!_bb.readByte(_it)) return false;
  return true;
}

kiwi::Array<uint8_t> *ByteArrayStruct::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const kiwi::Array<uint8_t> *ByteArrayStruct::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

kiwi::Array<uint8_t> &ByteArrayStruct::set_x(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 1; return _data_x = pool.array<uint8_t>(count);
}

bool ByteArrayStruct::encode(kiwi::ByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(_data_x.size());
  for (uint8_t &_it : _data_x) _bb.writeByte(_it);
  return true;
}

bool ByteArrayStruct::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  uint32_t _count;
  if (!_bb.readVarUint(_count)) return false;
  for (uint8_t &_it : set_x(_pool, _count)) if (!_bb.readByte(_it)) return false;
  return true;
}

kiwi::Array<int32_t> *IntArrayStruct::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const kiwi::Array<int32_t> *IntArrayStruct::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

kiwi::Array<int32_t> &IntArrayStruct::set_x(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 1; return _data_x = pool.array<int32_t>(count);
}

bool IntArrayStruct::encode(kiwi::ByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(_data_x.size());
  for (int32_t &_it : _data_x) _bb.writeVarInt(_it);
  return true;
}

bool IntArrayStruct::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  uint32_t _count;
  if (!_bb.readVarUint(_count)) return false;
  for (int32_t &_it : set_x(_pool, _count)) if (!_bb.readVarInt(_it)) return false;
  return true;
}

kiwi::Array<uint32_t> *UintArrayStruct::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const kiwi::Array<uint32_t> *UintArrayStruct::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

kiwi::Array<uint32_t> &UintArrayStruct::set_x(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 1; return _data_x = pool.array<uint32_t>(count);
}

bool UintArrayStruct::encode(kiwi::ByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(_data_x.size());
  for (uint32_t &_it : _data_x) _bb.writeVarUint(_it);
  return true;
}

bool UintArrayStruct::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  uint32_t _count;
  if (!_bb.readVarUint(_count)) return false;
  for (uint32_t &_it : set_x(_pool, _count)) if (!_bb.readVarUint(_it)) return false;
  return true;
}

kiwi::Array<float> *FloatArrayStruct::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const kiwi::Array<float> *FloatArrayStruct::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

kiwi::Array<float> &FloatArrayStruct::set_x(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 1; return _data_x = pool.array<float>(count);
}

bool FloatArrayStruct::encode(kiwi::ByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(_data_x.size());
  for (float &_it : _data_x) _bb.writeVarFloat(_it);
  return true;
}

bool FloatArrayStruct::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  uint32_t _count;
  if (!_bb.readVarUint(_count)) return false;
  for (float &_it : set_x(_pool, _count)) if (!_bb.readVarFloat(_it)) return false;
  return true;
}

kiwi::Array<kiwi::String> *StringArrayStruct::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const kiwi::Array<kiwi::String> *StringArrayStruct::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

kiwi::Array<kiwi::String> &StringArrayStruct::set_x(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 1; return _data_x = pool.array<kiwi::String>(count);
}

bool StringArrayStruct::encode(kiwi::ByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(_data_x.size());
  for (kiwi::String &_it : _data_x) _bb.writeString(_it.c_str());
  return true;
}

bool StringArrayStruct::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  uint32_t _count;
  if (!_bb.readVarUint(_count)) return false;
  for (kiwi::String &_it : set_x(_pool, _count)) if (!_bb.readString(_it, _pool)) return false;
  return true;
}

kiwi::Array<uint32_t> *CompoundArrayStruct::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const kiwi::Array<uint32_t> *CompoundArrayStruct::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

kiwi::Array<uint32_t> &CompoundArrayStruct::set_x(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 1; return _data_x = pool.array<uint32_t>(count);
}

kiwi::Array<uint32_t> *CompoundArrayStruct::y() {
  return _flags[0] & 2 ? &_data_y : nullptr;
}

const kiwi::Array<uint32_t> *CompoundArrayStruct::y() const {
  return _flags[0] & 2 ? &_data_y : nullptr;
}

kiwi::Array<uint32_t> &CompoundArrayStruct::set_y(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 2; return _data_y = pool.array<uint32_t>(count);
}

bool CompoundArrayStruct::encode(kiwi::ByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(_data_x.size());
  for (uint32_t &_it : _data_x) _bb.writeVarUint(_it);
  if (y() == nullptr) return false;
  _bb.writeVarUint(_data_y.size());
  for (uint32_t &_it : _data_y) _bb.writeVarUint(_it);
  return true;
}

bool CompoundArrayStruct::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  uint32_t _count;
  if (!_bb.readVarUint(_count)) return false;
  for (uint32_t &_it : set_x(_pool, _count)) if (!_bb.readVarUint(_it)) return false;
  if (!_bb.readVarUint(_count)) return false;
  for (uint32_t &_it : set_y(_pool, _count)) if (!_bb.readVarUint(_it)) return false;
  return true;
}

kiwi::Array<bool> *BoolArrayMessage::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const kiwi::Array<bool> *BoolArrayMessage::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

kiwi::Array<bool> &BoolArrayMessage::set_x(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 1; return _data_x = pool.array<bool>(count);
}

bool BoolArrayMessage::encode(kiwi::ByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_x.size());
    for (bool &_it : _data_x) _bb.writeByte(_it);
  }
  _bb.writeVarUint(0);
  return true;
}

bool BoolArrayMessage::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  uint32_t _count;
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1:
        if (!_bb.readVarUint(_count)) return false;
        for (bool &_it : set_x(_pool, _count)) if (!_bb.readByte(_it)) return false;
        break;
      default: return false;
    }
  }
}

kiwi::Array<uint8_t> *ByteArrayMessage::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const kiwi::Array<uint8_t> *ByteArrayMessage::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

kiwi::Array<uint8_t> &ByteArrayMessage::set_x(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 1; return _data_x = pool.array<uint8_t>(count);
}

bool ByteArrayMessage::encode(kiwi::ByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_x.size());
    for (uint8_t &_it : _data_x) _bb.writeByte(_it);
  }
  _bb.writeVarUint(0);
  return true;
}

bool ByteArrayMessage::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  uint32_t _count;
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1:
        if (!_bb.readVarUint(_count)) return false;
        for (uint8_t &_it : set_x(_pool, _count)) if (!_bb.readByte(_it)) return false;
        break;
      default: return false;
    }
  }
}

kiwi::Array<int32_t> *IntArrayMessage::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const kiwi::Array<int32_t> *IntArrayMessage::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

kiwi::Array<int32_t> &IntArrayMessage::set_x(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 1; return _data_x = pool.array<int32_t>(count);
}

bool IntArrayMessage::encode(kiwi::ByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_x.size());
    for (int32_t &_it : _data_x) _bb.writeVarInt(_it);
  }
  _bb.writeVarUint(0);
  return true;
}

bool IntArrayMessage::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  uint32_t _count;
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1:
        if (!_bb.readVarUint(_count)) return false;
        for (int32_t &_it : set_x(_pool, _count)) if (!_bb.readVarInt(_it)) return false;
        break;
      default: return false;
    }
  }
}

kiwi::Array<uint32_t> *UintArrayMessage::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const kiwi::Array<uint32_t> *UintArrayMessage::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

kiwi::Array<uint32_t> &UintArrayMessage::set_x(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 1; return _data_x = pool.array<uint32_t>(count);
}

bool UintArrayMessage::encode(kiwi::ByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_x.size());
    for (uint32_t &_it : _data_x) _bb.writeVarUint(_it);
  }
  _bb.writeVarUint(0);
  return true;
}

bool UintArrayMessage::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  uint32_t _count;
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1:
        if (!_bb.readVarUint(_count)) return false;
        for (uint32_t &_it : set_x(_pool, _count)) if (!_bb.readVarUint(_it)) return false;
        break;
      default: return false;
    }
  }
}

kiwi::Array<float> *FloatArrayMessage::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const kiwi::Array<float> *FloatArrayMessage::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

kiwi::Array<float> &FloatArrayMessage::set_x(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 1; return _data_x = pool.array<float>(count);
}

bool FloatArrayMessage::encode(kiwi::ByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_x.size());
    for (float &_it : _data_x) _bb.writeVarFloat(_it);
  }
  _bb.writeVarUint(0);
  return true;
}

bool FloatArrayMessage::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  uint32_t _count;
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1:
        if (!_bb.readVarUint(_count)) return false;
        for (float &_it : set_x(_pool, _count)) if (!_bb.readVarFloat(_it)) return false;
        break;
      default: return false;
    }
  }
}

kiwi::Array<kiwi::String> *StringArrayMessage::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const kiwi::Array<kiwi::String> *StringArrayMessage::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

kiwi::Array<kiwi::String> &StringArrayMessage::set_x(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 1; return _data_x = pool.array<kiwi::String>(count);
}

bool StringArrayMessage::encode(kiwi::ByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_x.size());
    for (kiwi::String &_it : _data_x) _bb.writeString(_it.c_str());
  }
  _bb.writeVarUint(0);
  return true;
}

bool StringArrayMessage::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  uint32_t _count;
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1:
        if (!_bb.readVarUint(_count)) return false;
        for (kiwi::String &_it : set_x(_pool, _count)) if (!_bb.readString(_it, _pool)) return false;
        break;
      default: return false;
    }
  }
}

kiwi::Array<uint32_t> *CompoundArrayMessage::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const kiwi::Array<uint32_t> *CompoundArrayMessage::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

kiwi::Array<uint32_t> &CompoundArrayMessage::set_x(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 1; return _data_x = pool.array<uint32_t>(count);
}

kiwi::Array<uint32_t> *CompoundArrayMessage::y() {
  return _flags[0] & 2 ? &_data_y : nullptr;
}

const kiwi::Array<uint32_t> *CompoundArrayMessage::y() const {
  return _flags[0] & 2 ? &_data_y : nullptr;
}

kiwi::Array<uint32_t> &CompoundArrayMessage::set_y(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 2; return _data_y = pool.array<uint32_t>(count);
}

bool CompoundArrayMessage::encode(kiwi::ByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_x.size());
    for (uint32_t &_it : _data_x) _bb.writeVarUint(_it);
  }
  if (y() != nullptr) {
    _bb.writeVarUint(2);
    _bb.writeVarUint(_data_y.size());
    for (uint32_t &_it : _data_y) _bb.writeVarUint(_it);
  }
  _bb.writeVarUint(0);
  return true;
}

bool CompoundArrayMessage::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  uint32_t _count;
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1:
        if (!_bb.readVarUint(_count)) return false;
        for (uint32_t &_it : set_x(_pool, _count)) if (!_bb.readVarUint(_it)) return false;
        break;
      case 2:
        if (!_bb.readVarUint(_count)) return false;
        for (uint32_t &_it : set_y(_pool, _count)) if (!_bb.readVarUint(_it)) return false;
        break;
      default: return false;
    }
  }
}

RecursiveMessage *RecursiveMessage::x() {
  return _data_x;
}

const RecursiveMessage *RecursiveMessage::x() const {
  return _data_x;
}

void RecursiveMessage::set_x(RecursiveMessage *value) {
  _data_x = value;
}

bool RecursiveMessage::encode(kiwi::ByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    if (!_data_x->encode(_bb)) return false;
  }
  _bb.writeVarUint(0);
  return true;
}

bool RecursiveMessage::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1:
        _data_x = _pool.allocate<RecursiveMessage>();
        if (!_data_x->decode(_bb, _pool)) return false;
        break;
      default: return false;
    }
  }
}

int32_t *RequiredField::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const int32_t *RequiredField::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

void RequiredField::set_x(const int32_t &value) {
  _flags[0] |= 1; _data_x = value;
}

bool RequiredField::encode(kiwi::ByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(1);
  _bb.writeVarInt(_data_x);
  _bb.writeVarUint(0);
  return true;
}

bool RequiredField::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        if (x() == nullptr) return false;
        return true;
      case 1:
        if (!_bb.readVarInt(_data_x)) return false;
        set_x(_data_x);
        break;
      default: return false;
    }
  }
}

#endif

}
