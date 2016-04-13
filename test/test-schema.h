#include "kiwi.h"

namespace test_cpp {

class BoolStructDecoder;
class BoolStructEncoder;
class ByteStructDecoder;
class ByteStructEncoder;
class IntStructDecoder;
class IntStructEncoder;
class UintStructDecoder;
class UintStructEncoder;
class FloatStructDecoder;
class FloatStructEncoder;
class StringStructDecoder;
class StringStructEncoder;
class CompoundStructDecoder;
class CompoundStructEncoder;
class NestedStructDecoder;
class NestedStructEncoder;
class BoolMessageDecoder;
class BoolMessageEncoder;
class ByteMessageDecoder;
class ByteMessageEncoder;
class IntMessageDecoder;
class IntMessageEncoder;
class UintMessageDecoder;
class UintMessageEncoder;
class FloatMessageDecoder;
class FloatMessageEncoder;
class StringMessageDecoder;
class StringMessageEncoder;
class CompoundMessageDecoder;
class CompoundMessageEncoder;
class NestedMessageDecoder;
class NestedMessageEncoder;

class BoolStructDecoder : public kiwi::Codec {
public:
  BoolStructDecoder() {}
  BoolStructDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  BoolStructDecoder(BoolStructDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  bool read_x(bool &value);
};

class BoolStructEncoder : public kiwi::Codec {
public:
  BoolStructEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  BoolStructEncoder(BoolStructEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~BoolStructEncoder();
  BoolStructEncoder &add_x(bool value);
};

class ByteStructDecoder : public kiwi::Codec {
public:
  ByteStructDecoder() {}
  ByteStructDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  ByteStructDecoder(ByteStructDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  bool read_x(uint8_t &value);
};

class ByteStructEncoder : public kiwi::Codec {
public:
  ByteStructEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  ByteStructEncoder(ByteStructEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~ByteStructEncoder();
  ByteStructEncoder &add_x(uint8_t value);
};

class IntStructDecoder : public kiwi::Codec {
public:
  IntStructDecoder() {}
  IntStructDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  IntStructDecoder(IntStructDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  bool read_x(int32_t &value);
};

class IntStructEncoder : public kiwi::Codec {
public:
  IntStructEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  IntStructEncoder(IntStructEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~IntStructEncoder();
  IntStructEncoder &add_x(int32_t value);
};

class UintStructDecoder : public kiwi::Codec {
public:
  UintStructDecoder() {}
  UintStructDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  UintStructDecoder(UintStructDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  bool read_x(uint32_t &value);
};

class UintStructEncoder : public kiwi::Codec {
public:
  UintStructEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  UintStructEncoder(UintStructEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~UintStructEncoder();
  UintStructEncoder &add_x(uint32_t value);
};

class FloatStructDecoder : public kiwi::Codec {
public:
  FloatStructDecoder() {}
  FloatStructDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  FloatStructDecoder(FloatStructDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  bool read_x(float &value);
};

class FloatStructEncoder : public kiwi::Codec {
public:
  FloatStructEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  FloatStructEncoder(FloatStructEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~FloatStructEncoder();
  FloatStructEncoder &add_x(float value);
};

class StringStructDecoder : public kiwi::Codec {
public:
  StringStructDecoder() {}
  StringStructDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  StringStructDecoder(StringStructDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  bool read_x(std::string &value);
};

class StringStructEncoder : public kiwi::Codec {
public:
  StringStructEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  StringStructEncoder(StringStructEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~StringStructEncoder();
  StringStructEncoder &add_x(const std::string &value);
};

class CompoundStructDecoder : public kiwi::Codec {
public:
  CompoundStructDecoder() {}
  CompoundStructDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  CompoundStructDecoder(CompoundStructDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  bool read_x(uint32_t &value);
  bool read_y(uint32_t &value);
};

class CompoundStructEncoder : public kiwi::Codec {
public:
  CompoundStructEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  CompoundStructEncoder(CompoundStructEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~CompoundStructEncoder();
  CompoundStructEncoder &add_x(uint32_t value);
  CompoundStructEncoder &add_y(uint32_t value);
};

class NestedStructDecoder : public kiwi::Codec {
public:
  NestedStructDecoder() {}
  NestedStructDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  NestedStructDecoder(NestedStructDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  bool read_a(uint32_t &value);
  bool read_b(CompoundStructDecoder &decoder);
  bool read_c(uint32_t &value);
};

class NestedStructEncoder : public kiwi::Codec {
public:
  NestedStructEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  NestedStructEncoder(NestedStructEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~NestedStructEncoder();
  NestedStructEncoder &add_a(uint32_t value);
  CompoundStructEncoder add_b();
  NestedStructEncoder &add_c(uint32_t value);
};

class BoolMessageDecoder : public kiwi::Codec {
public:
  BoolMessageDecoder() {}
  BoolMessageDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  BoolMessageDecoder(BoolMessageDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  enum Field {
    X = 1,
  };
  bool nextField(Field &value);
  bool read_x(bool &value);
};

class BoolMessageEncoder : public kiwi::Codec {
public:
  BoolMessageEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  BoolMessageEncoder(BoolMessageEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~BoolMessageEncoder();
  BoolMessageEncoder &add_x(bool value);
  BoolMessageEncoder &finish();
};

class ByteMessageDecoder : public kiwi::Codec {
public:
  ByteMessageDecoder() {}
  ByteMessageDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  ByteMessageDecoder(ByteMessageDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  enum Field {
    X = 1,
  };
  bool nextField(Field &value);
  bool read_x(uint8_t &value);
};

class ByteMessageEncoder : public kiwi::Codec {
public:
  ByteMessageEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  ByteMessageEncoder(ByteMessageEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~ByteMessageEncoder();
  ByteMessageEncoder &add_x(uint8_t value);
  ByteMessageEncoder &finish();
};

class IntMessageDecoder : public kiwi::Codec {
public:
  IntMessageDecoder() {}
  IntMessageDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  IntMessageDecoder(IntMessageDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  enum Field {
    X = 1,
  };
  bool nextField(Field &value);
  bool read_x(int32_t &value);
};

class IntMessageEncoder : public kiwi::Codec {
public:
  IntMessageEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  IntMessageEncoder(IntMessageEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~IntMessageEncoder();
  IntMessageEncoder &add_x(int32_t value);
  IntMessageEncoder &finish();
};

class UintMessageDecoder : public kiwi::Codec {
public:
  UintMessageDecoder() {}
  UintMessageDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  UintMessageDecoder(UintMessageDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  enum Field {
    X = 1,
  };
  bool nextField(Field &value);
  bool read_x(uint32_t &value);
};

class UintMessageEncoder : public kiwi::Codec {
public:
  UintMessageEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  UintMessageEncoder(UintMessageEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~UintMessageEncoder();
  UintMessageEncoder &add_x(uint32_t value);
  UintMessageEncoder &finish();
};

class FloatMessageDecoder : public kiwi::Codec {
public:
  FloatMessageDecoder() {}
  FloatMessageDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  FloatMessageDecoder(FloatMessageDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  enum Field {
    X = 1,
  };
  bool nextField(Field &value);
  bool read_x(float &value);
};

class FloatMessageEncoder : public kiwi::Codec {
public:
  FloatMessageEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  FloatMessageEncoder(FloatMessageEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~FloatMessageEncoder();
  FloatMessageEncoder &add_x(float value);
  FloatMessageEncoder &finish();
};

class StringMessageDecoder : public kiwi::Codec {
public:
  StringMessageDecoder() {}
  StringMessageDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  StringMessageDecoder(StringMessageDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  enum Field {
    X = 1,
  };
  bool nextField(Field &value);
  bool read_x(std::string &value);
};

class StringMessageEncoder : public kiwi::Codec {
public:
  StringMessageEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  StringMessageEncoder(StringMessageEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~StringMessageEncoder();
  StringMessageEncoder &add_x(const std::string &value);
  StringMessageEncoder &finish();
};

class CompoundMessageDecoder : public kiwi::Codec {
public:
  CompoundMessageDecoder() {}
  CompoundMessageDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  CompoundMessageDecoder(CompoundMessageDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  enum Field {
    X = 1,
    Y = 2,
  };
  bool nextField(Field &value);
  bool read_x(uint32_t &value);
  bool read_y(uint32_t &value);
};

class CompoundMessageEncoder : public kiwi::Codec {
public:
  CompoundMessageEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  CompoundMessageEncoder(CompoundMessageEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~CompoundMessageEncoder();
  CompoundMessageEncoder &add_x(uint32_t value);
  CompoundMessageEncoder &add_y(uint32_t value);
  CompoundMessageEncoder &finish();
};

class NestedMessageDecoder : public kiwi::Codec {
public:
  NestedMessageDecoder() {}
  NestedMessageDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  NestedMessageDecoder(NestedMessageDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  enum Field {
    A = 1,
    B = 2,
    C = 3,
  };
  bool nextField(Field &value);
  bool read_a(uint32_t &value);
  bool read_b(CompoundMessageDecoder &decoder);
  bool read_c(uint32_t &value);
};

class NestedMessageEncoder : public kiwi::Codec {
public:
  NestedMessageEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  NestedMessageEncoder(NestedMessageEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~NestedMessageEncoder();
  NestedMessageEncoder &add_a(uint32_t value);
  CompoundMessageEncoder add_b();
  NestedMessageEncoder &add_c(uint32_t value);
  NestedMessageEncoder &finish();
};

bool BoolStructDecoder::read_x(bool &value) {
  uint8_t byte;
  if (!_structReadByte(0, byte)) return false;
  value = byte;
  return true;
}

BoolStructEncoder &BoolStructEncoder::add_x(bool value) {
  _structWriteByte(0, value);
  return *this;
}

BoolStructEncoder::~BoolStructEncoder() {
  assert(!_bb || _nextField++ == 1); // Must set all fields
}

bool ByteStructDecoder::read_x(uint8_t &value) {
  return _structReadByte(0, value);
}

ByteStructEncoder &ByteStructEncoder::add_x(uint8_t value) {
  _structWriteByte(0, value);
  return *this;
}

ByteStructEncoder::~ByteStructEncoder() {
  assert(!_bb || _nextField++ == 1); // Must set all fields
}

bool IntStructDecoder::read_x(int32_t &value) {
  return _structReadVarInt(0, value);
}

IntStructEncoder &IntStructEncoder::add_x(int32_t value) {
  _structWriteVarInt(0, value);
  return *this;
}

IntStructEncoder::~IntStructEncoder() {
  assert(!_bb || _nextField++ == 1); // Must set all fields
}

bool UintStructDecoder::read_x(uint32_t &value) {
  return _structReadVarUint(0, value);
}

UintStructEncoder &UintStructEncoder::add_x(uint32_t value) {
  _structWriteVarUint(0, value);
  return *this;
}

UintStructEncoder::~UintStructEncoder() {
  assert(!_bb || _nextField++ == 1); // Must set all fields
}

bool FloatStructDecoder::read_x(float &value) {
  return _structReadFloat(0, value);
}

FloatStructEncoder &FloatStructEncoder::add_x(float value) {
  _structWriteFloat(0, value);
  return *this;
}

FloatStructEncoder::~FloatStructEncoder() {
  assert(!_bb || _nextField++ == 1); // Must set all fields
}

bool StringStructDecoder::read_x(std::string &value) {
  return _structReadString(0, value);
}

StringStructEncoder &StringStructEncoder::add_x(const std::string &value) {
  _structWriteString(0, value);
  return *this;
}

StringStructEncoder::~StringStructEncoder() {
  assert(!_bb || _nextField++ == 1); // Must set all fields
}

bool CompoundStructDecoder::read_x(uint32_t &value) {
  return _structReadVarUint(0, value);
}

bool CompoundStructDecoder::read_y(uint32_t &value) {
  return _structReadVarUint(1, value);
}

CompoundStructEncoder &CompoundStructEncoder::add_x(uint32_t value) {
  _structWriteVarUint(0, value);
  return *this;
}

CompoundStructEncoder &CompoundStructEncoder::add_y(uint32_t value) {
  _structWriteVarUint(1, value);
  return *this;
}

CompoundStructEncoder::~CompoundStructEncoder() {
  assert(!_bb || _nextField++ == 2); // Must set all fields
}

bool NestedStructDecoder::read_a(uint32_t &value) {
  return _structReadVarUint(0, value);
}

bool NestedStructDecoder::read_b(CompoundStructDecoder &decoder) {
  return _structReadFieldNested(1, decoder);
}

bool NestedStructDecoder::read_c(uint32_t &value) {
  return _structReadVarUint(2, value);
}

NestedStructEncoder &NestedStructEncoder::add_a(uint32_t value) {
  _structWriteVarUint(0, value);
  return *this;
}

CompoundStructEncoder NestedStructEncoder::add_b() {
  _structWriteField(1);
  return CompoundStructEncoder(*_bb);
}

NestedStructEncoder &NestedStructEncoder::add_c(uint32_t value) {
  _structWriteVarUint(2, value);
  return *this;
}

NestedStructEncoder::~NestedStructEncoder() {
  assert(!_bb || _nextField++ == 3); // Must set all fields
}

bool BoolMessageDecoder::nextField(Field &value) {
  if (!_messageReadField() || _nextField == 0) return false;
  value = (Field)_nextField;
  return true;
}

bool BoolMessageDecoder::read_x(bool &value) {
  uint8_t byte;
  if (!_messageReadByte(1, byte)) return false;
  value = byte;
  return true;
}

BoolMessageEncoder& BoolMessageEncoder::add_x(bool value) {
  _messageWriteByte(1, value);
  return *this;
}

BoolMessageEncoder &BoolMessageEncoder::finish() {
  _messageFinish();
  return *this;
}

BoolMessageEncoder::~BoolMessageEncoder() {
  assert(!_bb); // Must call finish()
}

bool ByteMessageDecoder::nextField(Field &value) {
  if (!_messageReadField() || _nextField == 0) return false;
  value = (Field)_nextField;
  return true;
}

bool ByteMessageDecoder::read_x(uint8_t &value) {
  return _messageReadByte(1, value);
}

ByteMessageEncoder& ByteMessageEncoder::add_x(uint8_t value) {
  _messageWriteByte(1, value);
  return *this;
}

ByteMessageEncoder &ByteMessageEncoder::finish() {
  _messageFinish();
  return *this;
}

ByteMessageEncoder::~ByteMessageEncoder() {
  assert(!_bb); // Must call finish()
}

bool IntMessageDecoder::nextField(Field &value) {
  if (!_messageReadField() || _nextField == 0) return false;
  value = (Field)_nextField;
  return true;
}

bool IntMessageDecoder::read_x(int32_t &value) {
  return _messageReadVarInt(1, value);
}

IntMessageEncoder& IntMessageEncoder::add_x(int32_t value) {
  _messageWriteVarInt(1, value);
  return *this;
}

IntMessageEncoder &IntMessageEncoder::finish() {
  _messageFinish();
  return *this;
}

IntMessageEncoder::~IntMessageEncoder() {
  assert(!_bb); // Must call finish()
}

bool UintMessageDecoder::nextField(Field &value) {
  if (!_messageReadField() || _nextField == 0) return false;
  value = (Field)_nextField;
  return true;
}

bool UintMessageDecoder::read_x(uint32_t &value) {
  return _messageReadVarUint(1, value);
}

UintMessageEncoder& UintMessageEncoder::add_x(uint32_t value) {
  _messageWriteVarUint(1, value);
  return *this;
}

UintMessageEncoder &UintMessageEncoder::finish() {
  _messageFinish();
  return *this;
}

UintMessageEncoder::~UintMessageEncoder() {
  assert(!_bb); // Must call finish()
}

bool FloatMessageDecoder::nextField(Field &value) {
  if (!_messageReadField() || _nextField == 0) return false;
  value = (Field)_nextField;
  return true;
}

bool FloatMessageDecoder::read_x(float &value) {
  return _messageReadFloat(1, value);
}

FloatMessageEncoder& FloatMessageEncoder::add_x(float value) {
  _messageWriteFloat(1, value);
  return *this;
}

FloatMessageEncoder &FloatMessageEncoder::finish() {
  _messageFinish();
  return *this;
}

FloatMessageEncoder::~FloatMessageEncoder() {
  assert(!_bb); // Must call finish()
}

bool StringMessageDecoder::nextField(Field &value) {
  if (!_messageReadField() || _nextField == 0) return false;
  value = (Field)_nextField;
  return true;
}

bool StringMessageDecoder::read_x(std::string &value) {
  return _messageReadString(1, value);
}

StringMessageEncoder& StringMessageEncoder::add_x(const std::string &value) {
  _messageWriteString(1, value);
  return *this;
}

StringMessageEncoder &StringMessageEncoder::finish() {
  _messageFinish();
  return *this;
}

StringMessageEncoder::~StringMessageEncoder() {
  assert(!_bb); // Must call finish()
}

bool CompoundMessageDecoder::nextField(Field &value) {
  if (!_messageReadField() || _nextField == 0) return false;
  value = (Field)_nextField;
  return true;
}

bool CompoundMessageDecoder::read_x(uint32_t &value) {
  return _messageReadVarUint(1, value);
}

bool CompoundMessageDecoder::read_y(uint32_t &value) {
  return _messageReadVarUint(2, value);
}

CompoundMessageEncoder& CompoundMessageEncoder::add_x(uint32_t value) {
  _messageWriteVarUint(1, value);
  return *this;
}

CompoundMessageEncoder& CompoundMessageEncoder::add_y(uint32_t value) {
  _messageWriteVarUint(2, value);
  return *this;
}

CompoundMessageEncoder &CompoundMessageEncoder::finish() {
  _messageFinish();
  return *this;
}

CompoundMessageEncoder::~CompoundMessageEncoder() {
  assert(!_bb); // Must call finish()
}

bool NestedMessageDecoder::nextField(Field &value) {
  if (!_messageReadField() || _nextField == 0) return false;
  value = (Field)_nextField;
  return true;
}

bool NestedMessageDecoder::read_a(uint32_t &value) {
  return _messageReadVarUint(1, value);
}

bool NestedMessageDecoder::read_b(CompoundMessageDecoder &decoder) {
  return _messageReadFieldNested(2, decoder);
}

bool NestedMessageDecoder::read_c(uint32_t &value) {
  return _messageReadVarUint(3, value);
}

NestedMessageEncoder& NestedMessageEncoder::add_a(uint32_t value) {
  _messageWriteVarUint(1, value);
  return *this;
}

CompoundMessageEncoder NestedMessageEncoder::add_b() {
  _messageWriteField(2);
  return CompoundMessageEncoder(*_bb);
}

NestedMessageEncoder& NestedMessageEncoder::add_c(uint32_t value) {
  _messageWriteVarUint(3, value);
  return *this;
}

NestedMessageEncoder &NestedMessageEncoder::finish() {
  _messageFinish();
  return *this;
}

NestedMessageEncoder::~NestedMessageEncoder() {
  assert(!_bb); // Must call finish()
}

}
