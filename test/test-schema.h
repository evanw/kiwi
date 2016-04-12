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

class ByteStructDecoder : kiwi::Codec {
public:
  ByteStructDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  ByteStructDecoder(ByteStructDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  ~ByteStructDecoder();
  bool read_x(uint8_t &value);
};

class ByteStructEncoder : kiwi::Codec {
public:
  ByteStructEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  ByteStructEncoder(ByteStructEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~ByteStructEncoder();
  ByteStructEncoder &add_x(uint8_t value);
};

class IntStructDecoder : kiwi::Codec {
public:
  IntStructDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  IntStructDecoder(IntStructDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  ~IntStructDecoder();
  bool read_x(int32_t &value);
};

class IntStructEncoder : kiwi::Codec {
public:
  IntStructEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  IntStructEncoder(IntStructEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~IntStructEncoder();
  IntStructEncoder &add_x(int32_t value);
};

class UintStructDecoder : kiwi::Codec {
public:
  UintStructDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  UintStructDecoder(UintStructDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  ~UintStructDecoder();
  bool read_x(uint32_t &value);
};

class UintStructEncoder : kiwi::Codec {
public:
  UintStructEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  UintStructEncoder(UintStructEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~UintStructEncoder();
  UintStructEncoder &add_x(uint32_t value);
};

class FloatStructDecoder : kiwi::Codec {
public:
  FloatStructDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  FloatStructDecoder(FloatStructDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  ~FloatStructDecoder();
  bool read_x(float &value);
};

class FloatStructEncoder : kiwi::Codec {
public:
  FloatStructEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  FloatStructEncoder(FloatStructEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~FloatStructEncoder();
  FloatStructEncoder &add_x(float value);
};

class StringStructDecoder : kiwi::Codec {
public:
  StringStructDecoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  StringStructDecoder(StringStructDecoder &&decoder) : kiwi::Codec(std::move(decoder)) {}
  ~StringStructDecoder();
  bool read_x(std::string &value);
};

class StringStructEncoder : kiwi::Codec {
public:
  StringStructEncoder(kiwi::ByteBuffer &bb) : kiwi::Codec(bb) {}
  StringStructEncoder(StringStructEncoder &&encoder) : kiwi::Codec(std::move(encoder)) {}
  ~StringStructEncoder();
  StringStructEncoder &add_x(const std::string &value);
};

bool ByteStructDecoder::read_x(uint8_t &value) {
  return _structReadByte(0, value);
}

ByteStructDecoder::~ByteStructDecoder() {
  assert(!_bb || _nextField++ == 1); // Must read all fields
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

IntStructDecoder::~IntStructDecoder() {
  assert(!_bb || _nextField++ == 1); // Must read all fields
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

UintStructDecoder::~UintStructDecoder() {
  assert(!_bb || _nextField++ == 1); // Must read all fields
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

FloatStructDecoder::~FloatStructDecoder() {
  assert(!_bb || _nextField++ == 1); // Must read all fields
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

StringStructDecoder::~StringStructDecoder() {
  assert(!_bb || _nextField++ == 1); // Must read all fields
}

StringStructEncoder &StringStructEncoder::add_x(const std::string &value) {
  _structWriteString(0, value);
  return *this;
}

StringStructEncoder::~StringStructEncoder() {
  assert(!_bb || _nextField++ == 1); // Must set all fields
}

}
