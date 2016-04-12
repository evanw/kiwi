#include "kiwi.h"

namespace test_cpp {

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
  bool read_x(int32_t &value);
  bool read_y(int32_t &value);
};

class CompoundStructEncoder : public kiwi::Codec {
public:
  CompoundStructEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  CompoundStructEncoder(CompoundStructEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~CompoundStructEncoder();
  CompoundStructEncoder &add_x(int32_t value);
  CompoundStructEncoder &add_y(int32_t value);
};

class NestedStructDecoder : public kiwi::Codec {
public:
  NestedStructDecoder() {}
  NestedStructDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  NestedStructDecoder(NestedStructDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  bool read_a(int32_t &value);
  bool read_b(CompoundStructDecoder &decoder);
  bool read_c(int32_t &value);
};

class NestedStructEncoder : public kiwi::Codec {
public:
  NestedStructEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  NestedStructEncoder(NestedStructEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~NestedStructEncoder();
  NestedStructEncoder &add_a(int32_t value);
  CompoundStructEncoder add_b();
  NestedStructEncoder &add_c(int32_t value);
};

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

bool CompoundStructDecoder::read_x(int32_t &value) {
  return _structReadVarInt(0, value);
}

bool CompoundStructDecoder::read_y(int32_t &value) {
  return _structReadVarInt(1, value);
}

CompoundStructEncoder &CompoundStructEncoder::add_x(int32_t value) {
  _structWriteVarInt(0, value);
  return *this;
}

CompoundStructEncoder &CompoundStructEncoder::add_y(int32_t value) {
  _structWriteVarInt(1, value);
  return *this;
}

CompoundStructEncoder::~CompoundStructEncoder() {
  assert(!_bb || _nextField++ == 2); // Must set all fields
}

bool NestedStructDecoder::read_a(int32_t &value) {
  return _structReadVarInt(0, value);
}

bool NestedStructDecoder::read_b(CompoundStructDecoder &decoder) {
  if (!_bb) return false;
  _structReadFieldNested(1, decoder);
  return true;
}

bool NestedStructDecoder::read_c(int32_t &value) {
  return _structReadVarInt(2, value);
}

NestedStructEncoder &NestedStructEncoder::add_a(int32_t value) {
  _structWriteVarInt(0, value);
  return *this;
}

CompoundStructEncoder NestedStructEncoder::add_b() {
  _structWriteField(1);
  return CompoundStructEncoder(*_bb);
}

NestedStructEncoder &NestedStructEncoder::add_c(int32_t value) {
  _structWriteVarInt(2, value);
  return *this;
}

NestedStructEncoder::~NestedStructEncoder() {
  assert(!_bb || _nextField++ == 3); // Must set all fields
}

}
