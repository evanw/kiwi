#include "kiwi.h"

namespace test1 {

#ifndef INCLUDE_TEST1_H
#define INCLUDE_TEST1_H

class BinarySchema {
public:
  template <typename InputByteBuffer> bool parse(InputByteBuffer &bb);
  const kiwi::BinarySchema &underlyingSchema() const { return _schema; }
  template <typename InputByteBuffer> bool skipMessageField(InputByteBuffer &bb, uint32_t id) const;

private:
  kiwi::BinarySchema _schema;
  uint32_t _indexMessage = 0;
};

class Struct;
class Message;

class Struct {
public:
  Struct() { (void)_flags; }

  float *a();
  const float *a() const;
  void set_a(const float &value);

  float *b();
  const float *b() const;
  void set_b(const float &value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  float _data_a = {};
  float _data_b = {};
};

class Message {
public:
  Message() { (void)_flags; }

  int32_t *x();
  const int32_t *x() const;
  void set_x(const int32_t &value);

  Struct *y();
  const Struct *y() const;
  void set_y(Struct *value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  Struct *_data_y = {};
  int32_t _data_x = {};
};

#endif
#ifdef IMPLEMENT_SCHEMA_H

template <typename InputByteBuffer> bool BinarySchema::parse(InputByteBuffer &bb) {
  if (!_schema.parse(bb)) return false;
  _schema.findDefinition("Message", _indexMessage);
  return true;
}

template <typename InputByteBuffer> bool BinarySchema::skipMessageField(InputByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexMessage, id);
}

float *Struct::a() {
  return _flags[0] & 1 ? &_data_a : nullptr;
}

const float *Struct::a() const {
  return _flags[0] & 1 ? &_data_a : nullptr;
}

void Struct::set_a(const float &value) {
  _flags[0] |= 1; _data_a = value;
}

float *Struct::b() {
  return _flags[0] & 2 ? &_data_b : nullptr;
}

const float *Struct::b() const {
  return _flags[0] & 2 ? &_data_b : nullptr;
}

void Struct::set_b(const float &value) {
  _flags[0] |= 2; _data_b = value;
}

template <typename OutputByteBuffer> bool Struct::encode(OutputByteBuffer &_bb) {
  if (a() == nullptr) return false;
  _bb.writeVarFloat(_data_a);
  if (b() == nullptr) return false;
  _bb.writeVarFloat(_data_b);
  return true;
}

template <typename InputByteBuffer> bool Struct::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  if (!_bb.readVarFloat(_data_a)) return false;
  set_a(_data_a);
  if (!_bb.readVarFloat(_data_b)) return false;
  set_b(_data_b);
  return true;
}

int32_t *Message::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const int32_t *Message::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

void Message::set_x(const int32_t &value) {
  _flags[0] |= 1; _data_x = value;
}

Struct *Message::y() {
  return _data_y;
}

const Struct *Message::y() const {
  return _data_y;
}

void Message::set_y(Struct *value) {
  _data_y = value;
}

template <typename OutputByteBuffer> bool Message::encode(OutputByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarInt(_data_x);
  }
  if (y() != nullptr) {
    _bb.writeVarUint(2);
    if (!_data_y->encode(_bb)) return false;
  }
  _bb.writeVarUint(0);
  return true;
}

template <typename InputByteBuffer> bool Message::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        if (!_bb.readVarInt(_data_x)) return false;
        set_x(_data_x);
        break;
      }
      case 2: {
        _data_y = _pool.allocate<Struct>();
        if (!_data_y->decode(_bb, _pool, _schema)) return false;
        break;
      }
      default: {
        if (!_schema || !_schema->skipMessageField(_bb, _type)) return false;
        break;
      }
    }
  }
}

#endif

}
