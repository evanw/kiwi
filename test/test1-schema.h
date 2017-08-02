#include "kiwi.h"

namespace test1 {

class BinarySchema {
public:
  bool parse(kiwi::ByteBuffer &bb);
  bool skipMessageField(kiwi::ByteBuffer &bb, uint32_t id) const;

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

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

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

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  int32_t _data_x = {};
  Struct *_data_y = {};
};

#ifdef IMPLEMENT_SCHEMA_H

bool BinarySchema::parse(kiwi::ByteBuffer &bb) {
  if (!_schema.parse(bb)) return false;
  _schema.findDefinition("Message", _indexMessage);
  return true;
}

bool BinarySchema::skipMessageField(kiwi::ByteBuffer &bb, uint32_t id) const {
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

bool Struct::encode(kiwi::ByteBuffer &_bb) {
  if (a() == nullptr) return false;
  _bb.writeVarFloat(_data_a);
  if (b() == nullptr) return false;
  _bb.writeVarFloat(_data_b);
  return true;
}

bool Struct::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
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

bool Message::encode(kiwi::ByteBuffer &_bb) {
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

bool Message::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
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
