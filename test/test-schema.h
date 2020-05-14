#include "kiwi.h"

namespace test {

#ifndef INCLUDE_TEST_H
#define INCLUDE_TEST_H

class BinarySchema {
public:
  template <typename InputByteBuffer> bool parse(InputByteBuffer &bb);
  const kiwi::BinarySchema &underlyingSchema() const { return _schema; }
  template <typename InputByteBuffer> bool skipBoolMessageField(InputByteBuffer &bb, uint32_t id) const;
  template <typename InputByteBuffer> bool skipByteMessageField(InputByteBuffer &bb, uint32_t id) const;
  template <typename InputByteBuffer> bool skipIntMessageField(InputByteBuffer &bb, uint32_t id) const;
  template <typename InputByteBuffer> bool skipUintMessageField(InputByteBuffer &bb, uint32_t id) const;
  template <typename InputByteBuffer> bool skipFloatMessageField(InputByteBuffer &bb, uint32_t id) const;
  template <typename InputByteBuffer> bool skipStringMessageField(InputByteBuffer &bb, uint32_t id) const;
  template <typename InputByteBuffer> bool skipCompoundMessageField(InputByteBuffer &bb, uint32_t id) const;
  template <typename InputByteBuffer> bool skipNestedMessageField(InputByteBuffer &bb, uint32_t id) const;
  template <typename InputByteBuffer> bool skipBoolArrayMessageField(InputByteBuffer &bb, uint32_t id) const;
  template <typename InputByteBuffer> bool skipByteArrayMessageField(InputByteBuffer &bb, uint32_t id) const;
  template <typename InputByteBuffer> bool skipIntArrayMessageField(InputByteBuffer &bb, uint32_t id) const;
  template <typename InputByteBuffer> bool skipUintArrayMessageField(InputByteBuffer &bb, uint32_t id) const;
  template <typename InputByteBuffer> bool skipFloatArrayMessageField(InputByteBuffer &bb, uint32_t id) const;
  template <typename InputByteBuffer> bool skipStringArrayMessageField(InputByteBuffer &bb, uint32_t id) const;
  template <typename InputByteBuffer> bool skipCompoundArrayMessageField(InputByteBuffer &bb, uint32_t id) const;
  template <typename InputByteBuffer> bool skipRecursiveMessageField(InputByteBuffer &bb, uint32_t id) const;
  template <typename InputByteBuffer> bool skipNonDeprecatedMessageField(InputByteBuffer &bb, uint32_t id) const;
  template <typename InputByteBuffer> bool skipDeprecatedMessageField(InputByteBuffer &bb, uint32_t id) const;

private:
  kiwi::BinarySchema _schema;
  uint32_t _indexBoolMessage = 0;
  uint32_t _indexByteMessage = 0;
  uint32_t _indexIntMessage = 0;
  uint32_t _indexUintMessage = 0;
  uint32_t _indexFloatMessage = 0;
  uint32_t _indexStringMessage = 0;
  uint32_t _indexCompoundMessage = 0;
  uint32_t _indexNestedMessage = 0;
  uint32_t _indexBoolArrayMessage = 0;
  uint32_t _indexByteArrayMessage = 0;
  uint32_t _indexIntArrayMessage = 0;
  uint32_t _indexUintArrayMessage = 0;
  uint32_t _indexFloatArrayMessage = 0;
  uint32_t _indexStringArrayMessage = 0;
  uint32_t _indexCompoundArrayMessage = 0;
  uint32_t _indexRecursiveMessage = 0;
  uint32_t _indexNonDeprecatedMessage = 0;
  uint32_t _indexDeprecatedMessage = 0;
};

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
class NonDeprecatedMessage;
class DeprecatedMessage;
class SortedStruct;

class EnumStruct {
public:
  EnumStruct() { (void)_flags; }

  Enum *x();
  const Enum *x() const;
  void set_x(const Enum &value);

  kiwi::Array<Enum> *y();
  const kiwi::Array<Enum> *y() const;
  kiwi::Array<Enum> &set_y(kiwi::MemoryPool &pool, uint32_t count);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  Enum _data_x = {};
  kiwi::Array<Enum> _data_y = {};
};

class BoolStruct {
public:
  BoolStruct() { (void)_flags; }

  bool *x();
  const bool *x() const;
  void set_x(const bool &value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  bool _data_x = {};
};

class ByteStruct {
public:
  ByteStruct() { (void)_flags; }

  uint8_t *x();
  const uint8_t *x() const;
  void set_x(const uint8_t &value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  uint8_t _data_x = {};
};

class IntStruct {
public:
  IntStruct() { (void)_flags; }

  int32_t *x();
  const int32_t *x() const;
  void set_x(const int32_t &value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  int32_t _data_x = {};
};

class UintStruct {
public:
  UintStruct() { (void)_flags; }

  uint32_t *x();
  const uint32_t *x() const;
  void set_x(const uint32_t &value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  uint32_t _data_x = {};
};

class FloatStruct {
public:
  FloatStruct() { (void)_flags; }

  float *x();
  const float *x() const;
  void set_x(const float &value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  float _data_x = {};
};

class StringStruct {
public:
  StringStruct() { (void)_flags; }

  kiwi::String *x();
  const kiwi::String *x() const;
  void set_x(const kiwi::String &value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::String _data_x = {};
};

class CompoundStruct {
public:
  CompoundStruct() { (void)_flags; }

  uint32_t *x();
  const uint32_t *x() const;
  void set_x(const uint32_t &value);

  uint32_t *y();
  const uint32_t *y() const;
  void set_y(const uint32_t &value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  uint32_t _data_x = {};
  uint32_t _data_y = {};
};

class NestedStruct {
public:
  NestedStruct() { (void)_flags; }

  uint32_t *a();
  const uint32_t *a() const;
  void set_a(const uint32_t &value);

  CompoundStruct *b();
  const CompoundStruct *b() const;
  void set_b(CompoundStruct *value);

  uint32_t *c();
  const uint32_t *c() const;
  void set_c(const uint32_t &value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  CompoundStruct *_data_b = {};
  uint32_t _data_a = {};
  uint32_t _data_c = {};
};

class BoolMessage {
public:
  BoolMessage() { (void)_flags; }

  bool *x();
  const bool *x() const;
  void set_x(const bool &value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  bool _data_x = {};
};

class ByteMessage {
public:
  ByteMessage() { (void)_flags; }

  uint8_t *x();
  const uint8_t *x() const;
  void set_x(const uint8_t &value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  uint8_t _data_x = {};
};

class IntMessage {
public:
  IntMessage() { (void)_flags; }

  int32_t *x();
  const int32_t *x() const;
  void set_x(const int32_t &value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  int32_t _data_x = {};
};

class UintMessage {
public:
  UintMessage() { (void)_flags; }

  uint32_t *x();
  const uint32_t *x() const;
  void set_x(const uint32_t &value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  uint32_t _data_x = {};
};

class FloatMessage {
public:
  FloatMessage() { (void)_flags; }

  float *x();
  const float *x() const;
  void set_x(const float &value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  float _data_x = {};
};

class StringMessage {
public:
  StringMessage() { (void)_flags; }

  kiwi::String *x();
  const kiwi::String *x() const;
  void set_x(const kiwi::String &value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::String _data_x = {};
};

class CompoundMessage {
public:
  CompoundMessage() { (void)_flags; }

  uint32_t *x();
  const uint32_t *x() const;
  void set_x(const uint32_t &value);

  uint32_t *y();
  const uint32_t *y() const;
  void set_y(const uint32_t &value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  uint32_t _data_x = {};
  uint32_t _data_y = {};
};

class NestedMessage {
public:
  NestedMessage() { (void)_flags; }

  uint32_t *a();
  const uint32_t *a() const;
  void set_a(const uint32_t &value);

  CompoundMessage *b();
  const CompoundMessage *b() const;
  void set_b(CompoundMessage *value);

  uint32_t *c();
  const uint32_t *c() const;
  void set_c(const uint32_t &value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  CompoundMessage *_data_b = {};
  uint32_t _data_a = {};
  uint32_t _data_c = {};
};

class BoolArrayStruct {
public:
  BoolArrayStruct() { (void)_flags; }

  kiwi::Array<bool> *x();
  const kiwi::Array<bool> *x() const;
  kiwi::Array<bool> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<bool> _data_x = {};
};

class ByteArrayStruct {
public:
  ByteArrayStruct() { (void)_flags; }

  kiwi::Array<uint8_t> *x();
  const kiwi::Array<uint8_t> *x() const;
  kiwi::Array<uint8_t> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint8_t> _data_x = {};
};

class IntArrayStruct {
public:
  IntArrayStruct() { (void)_flags; }

  kiwi::Array<int32_t> *x();
  const kiwi::Array<int32_t> *x() const;
  kiwi::Array<int32_t> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<int32_t> _data_x = {};
};

class UintArrayStruct {
public:
  UintArrayStruct() { (void)_flags; }

  kiwi::Array<uint32_t> *x();
  const kiwi::Array<uint32_t> *x() const;
  kiwi::Array<uint32_t> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint32_t> _data_x = {};
};

class FloatArrayStruct {
public:
  FloatArrayStruct() { (void)_flags; }

  kiwi::Array<float> *x();
  const kiwi::Array<float> *x() const;
  kiwi::Array<float> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<float> _data_x = {};
};

class StringArrayStruct {
public:
  StringArrayStruct() { (void)_flags; }

  kiwi::Array<kiwi::String> *x();
  const kiwi::Array<kiwi::String> *x() const;
  kiwi::Array<kiwi::String> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<kiwi::String> _data_x = {};
};

class CompoundArrayStruct {
public:
  CompoundArrayStruct() { (void)_flags; }

  kiwi::Array<uint32_t> *x();
  const kiwi::Array<uint32_t> *x() const;
  kiwi::Array<uint32_t> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  kiwi::Array<uint32_t> *y();
  const kiwi::Array<uint32_t> *y() const;
  kiwi::Array<uint32_t> &set_y(kiwi::MemoryPool &pool, uint32_t count);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint32_t> _data_x = {};
  kiwi::Array<uint32_t> _data_y = {};
};

class BoolArrayMessage {
public:
  BoolArrayMessage() { (void)_flags; }

  kiwi::Array<bool> *x();
  const kiwi::Array<bool> *x() const;
  kiwi::Array<bool> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<bool> _data_x = {};
};

class ByteArrayMessage {
public:
  ByteArrayMessage() { (void)_flags; }

  kiwi::Array<uint8_t> *x();
  const kiwi::Array<uint8_t> *x() const;
  kiwi::Array<uint8_t> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint8_t> _data_x = {};
};

class IntArrayMessage {
public:
  IntArrayMessage() { (void)_flags; }

  kiwi::Array<int32_t> *x();
  const kiwi::Array<int32_t> *x() const;
  kiwi::Array<int32_t> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<int32_t> _data_x = {};
};

class UintArrayMessage {
public:
  UintArrayMessage() { (void)_flags; }

  kiwi::Array<uint32_t> *x();
  const kiwi::Array<uint32_t> *x() const;
  kiwi::Array<uint32_t> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint32_t> _data_x = {};
};

class FloatArrayMessage {
public:
  FloatArrayMessage() { (void)_flags; }

  kiwi::Array<float> *x();
  const kiwi::Array<float> *x() const;
  kiwi::Array<float> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<float> _data_x = {};
};

class StringArrayMessage {
public:
  StringArrayMessage() { (void)_flags; }

  kiwi::Array<kiwi::String> *x();
  const kiwi::Array<kiwi::String> *x() const;
  kiwi::Array<kiwi::String> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<kiwi::String> _data_x = {};
};

class CompoundArrayMessage {
public:
  CompoundArrayMessage() { (void)_flags; }

  kiwi::Array<uint32_t> *x();
  const kiwi::Array<uint32_t> *x() const;
  kiwi::Array<uint32_t> &set_x(kiwi::MemoryPool &pool, uint32_t count);

  kiwi::Array<uint32_t> *y();
  const kiwi::Array<uint32_t> *y() const;
  kiwi::Array<uint32_t> &set_y(kiwi::MemoryPool &pool, uint32_t count);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint32_t> _data_x = {};
  kiwi::Array<uint32_t> _data_y = {};
};

class RecursiveMessage {
public:
  RecursiveMessage() { (void)_flags; }

  RecursiveMessage *x();
  const RecursiveMessage *x() const;
  void set_x(RecursiveMessage *value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  RecursiveMessage *_data_x = {};
};

class NonDeprecatedMessage {
public:
  NonDeprecatedMessage() { (void)_flags; }

  uint32_t *a();
  const uint32_t *a() const;
  void set_a(const uint32_t &value);

  uint32_t *b();
  const uint32_t *b() const;
  void set_b(const uint32_t &value);

  kiwi::Array<uint32_t> *c();
  const kiwi::Array<uint32_t> *c() const;
  kiwi::Array<uint32_t> &set_c(kiwi::MemoryPool &pool, uint32_t count);

  kiwi::Array<uint32_t> *d();
  const kiwi::Array<uint32_t> *d() const;
  kiwi::Array<uint32_t> &set_d(kiwi::MemoryPool &pool, uint32_t count);

  ByteStruct *e();
  const ByteStruct *e() const;
  void set_e(ByteStruct *value);

  ByteStruct *f();
  const ByteStruct *f() const;
  void set_f(ByteStruct *value);

  uint32_t *g();
  const uint32_t *g() const;
  void set_g(const uint32_t &value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint32_t> _data_c = {};
  kiwi::Array<uint32_t> _data_d = {};
  ByteStruct *_data_e = {};
  ByteStruct *_data_f = {};
  uint32_t _data_a = {};
  uint32_t _data_b = {};
  uint32_t _data_g = {};
};

class DeprecatedMessage {
public:
  DeprecatedMessage() { (void)_flags; }

  uint32_t *a();
  const uint32_t *a() const;
  void set_a(const uint32_t &value);

  kiwi::Array<uint32_t> *c();
  const kiwi::Array<uint32_t> *c() const;
  kiwi::Array<uint32_t> &set_c(kiwi::MemoryPool &pool, uint32_t count);

  ByteStruct *e();
  const ByteStruct *e() const;
  void set_e(ByteStruct *value);

  uint32_t *g();
  const uint32_t *g() const;
  void set_g(const uint32_t &value);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<uint32_t> _data_c = {};
  ByteStruct *_data_e = {};
  uint32_t _data_a = {};
  uint32_t _data_g = {};
};

class SortedStruct {
public:
  SortedStruct() { (void)_flags; }

  bool *a1();
  const bool *a1() const;
  void set_a1(const bool &value);

  uint8_t *b1();
  const uint8_t *b1() const;
  void set_b1(const uint8_t &value);

  int32_t *c1();
  const int32_t *c1() const;
  void set_c1(const int32_t &value);

  uint32_t *d1();
  const uint32_t *d1() const;
  void set_d1(const uint32_t &value);

  float *e1();
  const float *e1() const;
  void set_e1(const float &value);

  kiwi::String *f1();
  const kiwi::String *f1() const;
  void set_f1(const kiwi::String &value);

  bool *a2();
  const bool *a2() const;
  void set_a2(const bool &value);

  uint8_t *b2();
  const uint8_t *b2() const;
  void set_b2(const uint8_t &value);

  int32_t *c2();
  const int32_t *c2() const;
  void set_c2(const int32_t &value);

  uint32_t *d2();
  const uint32_t *d2() const;
  void set_d2(const uint32_t &value);

  float *e2();
  const float *e2() const;
  void set_e2(const float &value);

  kiwi::String *f2();
  const kiwi::String *f2() const;
  void set_f2(const kiwi::String &value);

  kiwi::Array<bool> *a3();
  const kiwi::Array<bool> *a3() const;
  kiwi::Array<bool> &set_a3(kiwi::MemoryPool &pool, uint32_t count);

  kiwi::Array<uint8_t> *b3();
  const kiwi::Array<uint8_t> *b3() const;
  kiwi::Array<uint8_t> &set_b3(kiwi::MemoryPool &pool, uint32_t count);

  kiwi::Array<int32_t> *c3();
  const kiwi::Array<int32_t> *c3() const;
  kiwi::Array<int32_t> &set_c3(kiwi::MemoryPool &pool, uint32_t count);

  kiwi::Array<uint32_t> *d3();
  const kiwi::Array<uint32_t> *d3() const;
  kiwi::Array<uint32_t> &set_d3(kiwi::MemoryPool &pool, uint32_t count);

  kiwi::Array<float> *e3();
  const kiwi::Array<float> *e3() const;
  kiwi::Array<float> &set_e3(kiwi::MemoryPool &pool, uint32_t count);

  kiwi::Array<kiwi::String> *f3();
  const kiwi::Array<kiwi::String> *f3() const;
  kiwi::Array<kiwi::String> &set_f3(kiwi::MemoryPool &pool, uint32_t count);

  template <typename OutputByteBuffer> bool encode(OutputByteBuffer &bb);
  template <typename InputByteBuffer> bool decode(InputByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::String _data_f1 = {};
  kiwi::String _data_f2 = {};
  kiwi::Array<bool> _data_a3 = {};
  kiwi::Array<uint8_t> _data_b3 = {};
  kiwi::Array<int32_t> _data_c3 = {};
  kiwi::Array<uint32_t> _data_d3 = {};
  kiwi::Array<float> _data_e3 = {};
  kiwi::Array<kiwi::String> _data_f3 = {};
  int32_t _data_c1 = {};
  uint32_t _data_d1 = {};
  float _data_e1 = {};
  int32_t _data_c2 = {};
  uint32_t _data_d2 = {};
  float _data_e2 = {};
  bool _data_a1 = {};
  uint8_t _data_b1 = {};
  bool _data_a2 = {};
  uint8_t _data_b2 = {};
};

#endif
#ifdef IMPLEMENT_SCHEMA_H

template <typename InputByteBuffer> bool BinarySchema::parse(InputByteBuffer &bb) {
  if (!_schema.parse(bb)) return false;
  _schema.findDefinition("BoolMessage", _indexBoolMessage);
  _schema.findDefinition("ByteMessage", _indexByteMessage);
  _schema.findDefinition("IntMessage", _indexIntMessage);
  _schema.findDefinition("UintMessage", _indexUintMessage);
  _schema.findDefinition("FloatMessage", _indexFloatMessage);
  _schema.findDefinition("StringMessage", _indexStringMessage);
  _schema.findDefinition("CompoundMessage", _indexCompoundMessage);
  _schema.findDefinition("NestedMessage", _indexNestedMessage);
  _schema.findDefinition("BoolArrayMessage", _indexBoolArrayMessage);
  _schema.findDefinition("ByteArrayMessage", _indexByteArrayMessage);
  _schema.findDefinition("IntArrayMessage", _indexIntArrayMessage);
  _schema.findDefinition("UintArrayMessage", _indexUintArrayMessage);
  _schema.findDefinition("FloatArrayMessage", _indexFloatArrayMessage);
  _schema.findDefinition("StringArrayMessage", _indexStringArrayMessage);
  _schema.findDefinition("CompoundArrayMessage", _indexCompoundArrayMessage);
  _schema.findDefinition("RecursiveMessage", _indexRecursiveMessage);
  _schema.findDefinition("NonDeprecatedMessage", _indexNonDeprecatedMessage);
  _schema.findDefinition("DeprecatedMessage", _indexDeprecatedMessage);
  return true;
}

template <typename InputByteBuffer> bool BinarySchema::skipBoolMessageField(InputByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexBoolMessage, id);
}

template <typename InputByteBuffer> bool BinarySchema::skipByteMessageField(InputByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexByteMessage, id);
}

template <typename InputByteBuffer> bool BinarySchema::skipIntMessageField(InputByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexIntMessage, id);
}

template <typename InputByteBuffer> bool BinarySchema::skipUintMessageField(InputByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexUintMessage, id);
}

template <typename InputByteBuffer> bool BinarySchema::skipFloatMessageField(InputByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexFloatMessage, id);
}

template <typename InputByteBuffer> bool BinarySchema::skipStringMessageField(InputByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexStringMessage, id);
}

template <typename InputByteBuffer> bool BinarySchema::skipCompoundMessageField(InputByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexCompoundMessage, id);
}

template <typename InputByteBuffer> bool BinarySchema::skipNestedMessageField(InputByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexNestedMessage, id);
}

template <typename InputByteBuffer> bool BinarySchema::skipBoolArrayMessageField(InputByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexBoolArrayMessage, id);
}

template <typename InputByteBuffer> bool BinarySchema::skipByteArrayMessageField(InputByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexByteArrayMessage, id);
}

template <typename InputByteBuffer> bool BinarySchema::skipIntArrayMessageField(InputByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexIntArrayMessage, id);
}

template <typename InputByteBuffer> bool BinarySchema::skipUintArrayMessageField(InputByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexUintArrayMessage, id);
}

template <typename InputByteBuffer> bool BinarySchema::skipFloatArrayMessageField(InputByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexFloatArrayMessage, id);
}

template <typename InputByteBuffer> bool BinarySchema::skipStringArrayMessageField(InputByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexStringArrayMessage, id);
}

template <typename InputByteBuffer> bool BinarySchema::skipCompoundArrayMessageField(InputByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexCompoundArrayMessage, id);
}

template <typename InputByteBuffer> bool BinarySchema::skipRecursiveMessageField(InputByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexRecursiveMessage, id);
}

template <typename InputByteBuffer> bool BinarySchema::skipNonDeprecatedMessageField(InputByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexNonDeprecatedMessage, id);
}

template <typename InputByteBuffer> bool BinarySchema::skipDeprecatedMessageField(InputByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexDeprecatedMessage, id);
}

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

template <typename OutputByteBuffer> bool EnumStruct::encode(OutputByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(static_cast<uint32_t>(_data_x));
  if (y() == nullptr) return false;
  _bb.writeVarUint(_data_y.size());
  for (Enum &_it : _data_y) _bb.writeVarUint(static_cast<uint32_t>(_it));
  return true;
}

template <typename InputByteBuffer> bool EnumStruct::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
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

template <typename OutputByteBuffer> bool BoolStruct::encode(OutputByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeByte(_data_x);
  return true;
}

template <typename InputByteBuffer> bool BoolStruct::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
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

template <typename OutputByteBuffer> bool ByteStruct::encode(OutputByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeByte(_data_x);
  return true;
}

template <typename InputByteBuffer> bool ByteStruct::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
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

template <typename OutputByteBuffer> bool IntStruct::encode(OutputByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarInt(_data_x);
  return true;
}

template <typename InputByteBuffer> bool IntStruct::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
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

template <typename OutputByteBuffer> bool UintStruct::encode(OutputByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(_data_x);
  return true;
}

template <typename InputByteBuffer> bool UintStruct::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
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

template <typename OutputByteBuffer> bool FloatStruct::encode(OutputByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarFloat(_data_x);
  return true;
}

template <typename InputByteBuffer> bool FloatStruct::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
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

template <typename OutputByteBuffer> bool StringStruct::encode(OutputByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeString(_data_x.c_str());
  return true;
}

template <typename InputByteBuffer> bool StringStruct::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
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

template <typename OutputByteBuffer> bool CompoundStruct::encode(OutputByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(_data_x);
  if (y() == nullptr) return false;
  _bb.writeVarUint(_data_y);
  return true;
}

template <typename InputByteBuffer> bool CompoundStruct::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
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

template <typename OutputByteBuffer> bool NestedStruct::encode(OutputByteBuffer &_bb) {
  if (a() == nullptr) return false;
  _bb.writeVarUint(_data_a);
  if (b() == nullptr) return false;
  if (!_data_b->encode(_bb)) return false;
  if (c() == nullptr) return false;
  _bb.writeVarUint(_data_c);
  return true;
}

template <typename InputByteBuffer> bool NestedStruct::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  if (!_bb.readVarUint(_data_a)) return false;
  set_a(_data_a);
  _data_b = _pool.allocate<CompoundStruct>();
  if (!_data_b->decode(_bb, _pool, _schema)) return false;
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

template <typename OutputByteBuffer> bool BoolMessage::encode(OutputByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeByte(_data_x);
  }
  _bb.writeVarUint(0);
  return true;
}

template <typename InputByteBuffer> bool BoolMessage::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        if (!_bb.readByte(_data_x)) return false;
        set_x(_data_x);
        break;
      }
      default: {
        if (!_schema || !_schema->skipBoolMessageField(_bb, _type)) return false;
        break;
      }
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

template <typename OutputByteBuffer> bool ByteMessage::encode(OutputByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeByte(_data_x);
  }
  _bb.writeVarUint(0);
  return true;
}

template <typename InputByteBuffer> bool ByteMessage::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        if (!_bb.readByte(_data_x)) return false;
        set_x(_data_x);
        break;
      }
      default: {
        if (!_schema || !_schema->skipByteMessageField(_bb, _type)) return false;
        break;
      }
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

template <typename OutputByteBuffer> bool IntMessage::encode(OutputByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarInt(_data_x);
  }
  _bb.writeVarUint(0);
  return true;
}

template <typename InputByteBuffer> bool IntMessage::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
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
      default: {
        if (!_schema || !_schema->skipIntMessageField(_bb, _type)) return false;
        break;
      }
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

template <typename OutputByteBuffer> bool UintMessage::encode(OutputByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_x);
  }
  _bb.writeVarUint(0);
  return true;
}

template <typename InputByteBuffer> bool UintMessage::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        if (!_bb.readVarUint(_data_x)) return false;
        set_x(_data_x);
        break;
      }
      default: {
        if (!_schema || !_schema->skipUintMessageField(_bb, _type)) return false;
        break;
      }
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

template <typename OutputByteBuffer> bool FloatMessage::encode(OutputByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarFloat(_data_x);
  }
  _bb.writeVarUint(0);
  return true;
}

template <typename InputByteBuffer> bool FloatMessage::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        if (!_bb.readVarFloat(_data_x)) return false;
        set_x(_data_x);
        break;
      }
      default: {
        if (!_schema || !_schema->skipFloatMessageField(_bb, _type)) return false;
        break;
      }
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

template <typename OutputByteBuffer> bool StringMessage::encode(OutputByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeString(_data_x.c_str());
  }
  _bb.writeVarUint(0);
  return true;
}

template <typename InputByteBuffer> bool StringMessage::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        if (!_bb.readString(_data_x, _pool)) return false;
        set_x(_data_x);
        break;
      }
      default: {
        if (!_schema || !_schema->skipStringMessageField(_bb, _type)) return false;
        break;
      }
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

template <typename OutputByteBuffer> bool CompoundMessage::encode(OutputByteBuffer &_bb) {
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

template <typename InputByteBuffer> bool CompoundMessage::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        if (!_bb.readVarUint(_data_x)) return false;
        set_x(_data_x);
        break;
      }
      case 2: {
        if (!_bb.readVarUint(_data_y)) return false;
        set_y(_data_y);
        break;
      }
      default: {
        if (!_schema || !_schema->skipCompoundMessageField(_bb, _type)) return false;
        break;
      }
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

template <typename OutputByteBuffer> bool NestedMessage::encode(OutputByteBuffer &_bb) {
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

template <typename InputByteBuffer> bool NestedMessage::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        if (!_bb.readVarUint(_data_a)) return false;
        set_a(_data_a);
        break;
      }
      case 2: {
        _data_b = _pool.allocate<CompoundMessage>();
        if (!_data_b->decode(_bb, _pool, _schema)) return false;
        break;
      }
      case 3: {
        if (!_bb.readVarUint(_data_c)) return false;
        set_c(_data_c);
        break;
      }
      default: {
        if (!_schema || !_schema->skipNestedMessageField(_bb, _type)) return false;
        break;
      }
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

template <typename OutputByteBuffer> bool BoolArrayStruct::encode(OutputByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(_data_x.size());
  for (bool &_it : _data_x) _bb.writeByte(_it);
  return true;
}

template <typename InputByteBuffer> bool BoolArrayStruct::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
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

template <typename OutputByteBuffer> bool ByteArrayStruct::encode(OutputByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(_data_x.size());
  for (uint8_t &_it : _data_x) _bb.writeByte(_it);
  return true;
}

template <typename InputByteBuffer> bool ByteArrayStruct::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
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

template <typename OutputByteBuffer> bool IntArrayStruct::encode(OutputByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(_data_x.size());
  for (int32_t &_it : _data_x) _bb.writeVarInt(_it);
  return true;
}

template <typename InputByteBuffer> bool IntArrayStruct::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
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

template <typename OutputByteBuffer> bool UintArrayStruct::encode(OutputByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(_data_x.size());
  for (uint32_t &_it : _data_x) _bb.writeVarUint(_it);
  return true;
}

template <typename InputByteBuffer> bool UintArrayStruct::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
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

template <typename OutputByteBuffer> bool FloatArrayStruct::encode(OutputByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(_data_x.size());
  for (float &_it : _data_x) _bb.writeVarFloat(_it);
  return true;
}

template <typename InputByteBuffer> bool FloatArrayStruct::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
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

template <typename OutputByteBuffer> bool StringArrayStruct::encode(OutputByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(_data_x.size());
  for (kiwi::String &_it : _data_x) _bb.writeString(_it.c_str());
  return true;
}

template <typename InputByteBuffer> bool StringArrayStruct::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
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

template <typename OutputByteBuffer> bool CompoundArrayStruct::encode(OutputByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarUint(_data_x.size());
  for (uint32_t &_it : _data_x) _bb.writeVarUint(_it);
  if (y() == nullptr) return false;
  _bb.writeVarUint(_data_y.size());
  for (uint32_t &_it : _data_y) _bb.writeVarUint(_it);
  return true;
}

template <typename InputByteBuffer> bool CompoundArrayStruct::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
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

template <typename OutputByteBuffer> bool BoolArrayMessage::encode(OutputByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_x.size());
    for (bool &_it : _data_x) _bb.writeByte(_it);
  }
  _bb.writeVarUint(0);
  return true;
}

template <typename InputByteBuffer> bool BoolArrayMessage::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  uint32_t _count;
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        if (!_bb.readVarUint(_count)) return false;
        for (bool &_it : set_x(_pool, _count)) if (!_bb.readByte(_it)) return false;
        break;
      }
      default: {
        if (!_schema || !_schema->skipBoolArrayMessageField(_bb, _type)) return false;
        break;
      }
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

template <typename OutputByteBuffer> bool ByteArrayMessage::encode(OutputByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_x.size());
    for (uint8_t &_it : _data_x) _bb.writeByte(_it);
  }
  _bb.writeVarUint(0);
  return true;
}

template <typename InputByteBuffer> bool ByteArrayMessage::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  uint32_t _count;
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        if (!_bb.readVarUint(_count)) return false;
        for (uint8_t &_it : set_x(_pool, _count)) if (!_bb.readByte(_it)) return false;
        break;
      }
      default: {
        if (!_schema || !_schema->skipByteArrayMessageField(_bb, _type)) return false;
        break;
      }
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

template <typename OutputByteBuffer> bool IntArrayMessage::encode(OutputByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_x.size());
    for (int32_t &_it : _data_x) _bb.writeVarInt(_it);
  }
  _bb.writeVarUint(0);
  return true;
}

template <typename InputByteBuffer> bool IntArrayMessage::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  uint32_t _count;
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        if (!_bb.readVarUint(_count)) return false;
        for (int32_t &_it : set_x(_pool, _count)) if (!_bb.readVarInt(_it)) return false;
        break;
      }
      default: {
        if (!_schema || !_schema->skipIntArrayMessageField(_bb, _type)) return false;
        break;
      }
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

template <typename OutputByteBuffer> bool UintArrayMessage::encode(OutputByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_x.size());
    for (uint32_t &_it : _data_x) _bb.writeVarUint(_it);
  }
  _bb.writeVarUint(0);
  return true;
}

template <typename InputByteBuffer> bool UintArrayMessage::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  uint32_t _count;
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        if (!_bb.readVarUint(_count)) return false;
        for (uint32_t &_it : set_x(_pool, _count)) if (!_bb.readVarUint(_it)) return false;
        break;
      }
      default: {
        if (!_schema || !_schema->skipUintArrayMessageField(_bb, _type)) return false;
        break;
      }
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

template <typename OutputByteBuffer> bool FloatArrayMessage::encode(OutputByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_x.size());
    for (float &_it : _data_x) _bb.writeVarFloat(_it);
  }
  _bb.writeVarUint(0);
  return true;
}

template <typename InputByteBuffer> bool FloatArrayMessage::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  uint32_t _count;
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        if (!_bb.readVarUint(_count)) return false;
        for (float &_it : set_x(_pool, _count)) if (!_bb.readVarFloat(_it)) return false;
        break;
      }
      default: {
        if (!_schema || !_schema->skipFloatArrayMessageField(_bb, _type)) return false;
        break;
      }
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

template <typename OutputByteBuffer> bool StringArrayMessage::encode(OutputByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_x.size());
    for (kiwi::String &_it : _data_x) _bb.writeString(_it.c_str());
  }
  _bb.writeVarUint(0);
  return true;
}

template <typename InputByteBuffer> bool StringArrayMessage::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  uint32_t _count;
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        if (!_bb.readVarUint(_count)) return false;
        for (kiwi::String &_it : set_x(_pool, _count)) if (!_bb.readString(_it, _pool)) return false;
        break;
      }
      default: {
        if (!_schema || !_schema->skipStringArrayMessageField(_bb, _type)) return false;
        break;
      }
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

template <typename OutputByteBuffer> bool CompoundArrayMessage::encode(OutputByteBuffer &_bb) {
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

template <typename InputByteBuffer> bool CompoundArrayMessage::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  uint32_t _count;
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        if (!_bb.readVarUint(_count)) return false;
        for (uint32_t &_it : set_x(_pool, _count)) if (!_bb.readVarUint(_it)) return false;
        break;
      }
      case 2: {
        if (!_bb.readVarUint(_count)) return false;
        for (uint32_t &_it : set_y(_pool, _count)) if (!_bb.readVarUint(_it)) return false;
        break;
      }
      default: {
        if (!_schema || !_schema->skipCompoundArrayMessageField(_bb, _type)) return false;
        break;
      }
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

template <typename OutputByteBuffer> bool RecursiveMessage::encode(OutputByteBuffer &_bb) {
  if (x() != nullptr) {
    _bb.writeVarUint(1);
    if (!_data_x->encode(_bb)) return false;
  }
  _bb.writeVarUint(0);
  return true;
}

template <typename InputByteBuffer> bool RecursiveMessage::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        _data_x = _pool.allocate<RecursiveMessage>();
        if (!_data_x->decode(_bb, _pool, _schema)) return false;
        break;
      }
      default: {
        if (!_schema || !_schema->skipRecursiveMessageField(_bb, _type)) return false;
        break;
      }
    }
  }
}

uint32_t *NonDeprecatedMessage::a() {
  return _flags[0] & 1 ? &_data_a : nullptr;
}

const uint32_t *NonDeprecatedMessage::a() const {
  return _flags[0] & 1 ? &_data_a : nullptr;
}

void NonDeprecatedMessage::set_a(const uint32_t &value) {
  _flags[0] |= 1; _data_a = value;
}

uint32_t *NonDeprecatedMessage::b() {
  return _flags[0] & 2 ? &_data_b : nullptr;
}

const uint32_t *NonDeprecatedMessage::b() const {
  return _flags[0] & 2 ? &_data_b : nullptr;
}

void NonDeprecatedMessage::set_b(const uint32_t &value) {
  _flags[0] |= 2; _data_b = value;
}

kiwi::Array<uint32_t> *NonDeprecatedMessage::c() {
  return _flags[0] & 4 ? &_data_c : nullptr;
}

const kiwi::Array<uint32_t> *NonDeprecatedMessage::c() const {
  return _flags[0] & 4 ? &_data_c : nullptr;
}

kiwi::Array<uint32_t> &NonDeprecatedMessage::set_c(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 4; return _data_c = pool.array<uint32_t>(count);
}

kiwi::Array<uint32_t> *NonDeprecatedMessage::d() {
  return _flags[0] & 8 ? &_data_d : nullptr;
}

const kiwi::Array<uint32_t> *NonDeprecatedMessage::d() const {
  return _flags[0] & 8 ? &_data_d : nullptr;
}

kiwi::Array<uint32_t> &NonDeprecatedMessage::set_d(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 8; return _data_d = pool.array<uint32_t>(count);
}

ByteStruct *NonDeprecatedMessage::e() {
  return _data_e;
}

const ByteStruct *NonDeprecatedMessage::e() const {
  return _data_e;
}

void NonDeprecatedMessage::set_e(ByteStruct *value) {
  _data_e = value;
}

ByteStruct *NonDeprecatedMessage::f() {
  return _data_f;
}

const ByteStruct *NonDeprecatedMessage::f() const {
  return _data_f;
}

void NonDeprecatedMessage::set_f(ByteStruct *value) {
  _data_f = value;
}

uint32_t *NonDeprecatedMessage::g() {
  return _flags[0] & 64 ? &_data_g : nullptr;
}

const uint32_t *NonDeprecatedMessage::g() const {
  return _flags[0] & 64 ? &_data_g : nullptr;
}

void NonDeprecatedMessage::set_g(const uint32_t &value) {
  _flags[0] |= 64; _data_g = value;
}

template <typename OutputByteBuffer> bool NonDeprecatedMessage::encode(OutputByteBuffer &_bb) {
  if (a() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_a);
  }
  if (b() != nullptr) {
    _bb.writeVarUint(2);
    _bb.writeVarUint(_data_b);
  }
  if (c() != nullptr) {
    _bb.writeVarUint(3);
    _bb.writeVarUint(_data_c.size());
    for (uint32_t &_it : _data_c) _bb.writeVarUint(_it);
  }
  if (d() != nullptr) {
    _bb.writeVarUint(4);
    _bb.writeVarUint(_data_d.size());
    for (uint32_t &_it : _data_d) _bb.writeVarUint(_it);
  }
  if (e() != nullptr) {
    _bb.writeVarUint(5);
    if (!_data_e->encode(_bb)) return false;
  }
  if (f() != nullptr) {
    _bb.writeVarUint(6);
    if (!_data_f->encode(_bb)) return false;
  }
  if (g() != nullptr) {
    _bb.writeVarUint(7);
    _bb.writeVarUint(_data_g);
  }
  _bb.writeVarUint(0);
  return true;
}

template <typename InputByteBuffer> bool NonDeprecatedMessage::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  uint32_t _count;
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        if (!_bb.readVarUint(_data_a)) return false;
        set_a(_data_a);
        break;
      }
      case 2: {
        if (!_bb.readVarUint(_data_b)) return false;
        set_b(_data_b);
        break;
      }
      case 3: {
        if (!_bb.readVarUint(_count)) return false;
        for (uint32_t &_it : set_c(_pool, _count)) if (!_bb.readVarUint(_it)) return false;
        break;
      }
      case 4: {
        if (!_bb.readVarUint(_count)) return false;
        for (uint32_t &_it : set_d(_pool, _count)) if (!_bb.readVarUint(_it)) return false;
        break;
      }
      case 5: {
        _data_e = _pool.allocate<ByteStruct>();
        if (!_data_e->decode(_bb, _pool, _schema)) return false;
        break;
      }
      case 6: {
        _data_f = _pool.allocate<ByteStruct>();
        if (!_data_f->decode(_bb, _pool, _schema)) return false;
        break;
      }
      case 7: {
        if (!_bb.readVarUint(_data_g)) return false;
        set_g(_data_g);
        break;
      }
      default: {
        if (!_schema || !_schema->skipNonDeprecatedMessageField(_bb, _type)) return false;
        break;
      }
    }
  }
}

uint32_t *DeprecatedMessage::a() {
  return _flags[0] & 1 ? &_data_a : nullptr;
}

const uint32_t *DeprecatedMessage::a() const {
  return _flags[0] & 1 ? &_data_a : nullptr;
}

void DeprecatedMessage::set_a(const uint32_t &value) {
  _flags[0] |= 1; _data_a = value;
}

kiwi::Array<uint32_t> *DeprecatedMessage::c() {
  return _flags[0] & 4 ? &_data_c : nullptr;
}

const kiwi::Array<uint32_t> *DeprecatedMessage::c() const {
  return _flags[0] & 4 ? &_data_c : nullptr;
}

kiwi::Array<uint32_t> &DeprecatedMessage::set_c(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 4; return _data_c = pool.array<uint32_t>(count);
}

ByteStruct *DeprecatedMessage::e() {
  return _data_e;
}

const ByteStruct *DeprecatedMessage::e() const {
  return _data_e;
}

void DeprecatedMessage::set_e(ByteStruct *value) {
  _data_e = value;
}

uint32_t *DeprecatedMessage::g() {
  return _flags[0] & 64 ? &_data_g : nullptr;
}

const uint32_t *DeprecatedMessage::g() const {
  return _flags[0] & 64 ? &_data_g : nullptr;
}

void DeprecatedMessage::set_g(const uint32_t &value) {
  _flags[0] |= 64; _data_g = value;
}

template <typename OutputByteBuffer> bool DeprecatedMessage::encode(OutputByteBuffer &_bb) {
  if (a() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_a);
  }
  if (c() != nullptr) {
    _bb.writeVarUint(3);
    _bb.writeVarUint(_data_c.size());
    for (uint32_t &_it : _data_c) _bb.writeVarUint(_it);
  }
  if (e() != nullptr) {
    _bb.writeVarUint(5);
    if (!_data_e->encode(_bb)) return false;
  }
  if (g() != nullptr) {
    _bb.writeVarUint(7);
    _bb.writeVarUint(_data_g);
  }
  _bb.writeVarUint(0);
  return true;
}

template <typename InputByteBuffer> bool DeprecatedMessage::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  uint32_t _count;
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        if (!_bb.readVarUint(_data_a)) return false;
        set_a(_data_a);
        break;
      }
      case 2: {
        uint32_t _data_b = {};
        if (!_bb.readVarUint(_data_b)) return false;
        break;
      }
      case 3: {
        if (!_bb.readVarUint(_count)) return false;
        for (uint32_t &_it : set_c(_pool, _count)) if (!_bb.readVarUint(_it)) return false;
        break;
      }
      case 4: {
        if (!_bb.readVarUint(_count)) return false;
        for (uint32_t &_it : _pool.array<uint32_t>(_count)) if (!_bb.readVarUint(_it)) return false;
        break;
      }
      case 5: {
        _data_e = _pool.allocate<ByteStruct>();
        if (!_data_e->decode(_bb, _pool, _schema)) return false;
        break;
      }
      case 6: {
        ByteStruct *_data_f = _pool.allocate<ByteStruct>();
        if (!_data_f->decode(_bb, _pool, _schema)) return false;
        break;
      }
      case 7: {
        if (!_bb.readVarUint(_data_g)) return false;
        set_g(_data_g);
        break;
      }
      default: {
        if (!_schema || !_schema->skipDeprecatedMessageField(_bb, _type)) return false;
        break;
      }
    }
  }
}

bool *SortedStruct::a1() {
  return _flags[0] & 1 ? &_data_a1 : nullptr;
}

const bool *SortedStruct::a1() const {
  return _flags[0] & 1 ? &_data_a1 : nullptr;
}

void SortedStruct::set_a1(const bool &value) {
  _flags[0] |= 1; _data_a1 = value;
}

uint8_t *SortedStruct::b1() {
  return _flags[0] & 2 ? &_data_b1 : nullptr;
}

const uint8_t *SortedStruct::b1() const {
  return _flags[0] & 2 ? &_data_b1 : nullptr;
}

void SortedStruct::set_b1(const uint8_t &value) {
  _flags[0] |= 2; _data_b1 = value;
}

int32_t *SortedStruct::c1() {
  return _flags[0] & 4 ? &_data_c1 : nullptr;
}

const int32_t *SortedStruct::c1() const {
  return _flags[0] & 4 ? &_data_c1 : nullptr;
}

void SortedStruct::set_c1(const int32_t &value) {
  _flags[0] |= 4; _data_c1 = value;
}

uint32_t *SortedStruct::d1() {
  return _flags[0] & 8 ? &_data_d1 : nullptr;
}

const uint32_t *SortedStruct::d1() const {
  return _flags[0] & 8 ? &_data_d1 : nullptr;
}

void SortedStruct::set_d1(const uint32_t &value) {
  _flags[0] |= 8; _data_d1 = value;
}

float *SortedStruct::e1() {
  return _flags[0] & 16 ? &_data_e1 : nullptr;
}

const float *SortedStruct::e1() const {
  return _flags[0] & 16 ? &_data_e1 : nullptr;
}

void SortedStruct::set_e1(const float &value) {
  _flags[0] |= 16; _data_e1 = value;
}

kiwi::String *SortedStruct::f1() {
  return _flags[0] & 32 ? &_data_f1 : nullptr;
}

const kiwi::String *SortedStruct::f1() const {
  return _flags[0] & 32 ? &_data_f1 : nullptr;
}

void SortedStruct::set_f1(const kiwi::String &value) {
  _flags[0] |= 32; _data_f1 = value;
}

bool *SortedStruct::a2() {
  return _flags[0] & 64 ? &_data_a2 : nullptr;
}

const bool *SortedStruct::a2() const {
  return _flags[0] & 64 ? &_data_a2 : nullptr;
}

void SortedStruct::set_a2(const bool &value) {
  _flags[0] |= 64; _data_a2 = value;
}

uint8_t *SortedStruct::b2() {
  return _flags[0] & 128 ? &_data_b2 : nullptr;
}

const uint8_t *SortedStruct::b2() const {
  return _flags[0] & 128 ? &_data_b2 : nullptr;
}

void SortedStruct::set_b2(const uint8_t &value) {
  _flags[0] |= 128; _data_b2 = value;
}

int32_t *SortedStruct::c2() {
  return _flags[0] & 256 ? &_data_c2 : nullptr;
}

const int32_t *SortedStruct::c2() const {
  return _flags[0] & 256 ? &_data_c2 : nullptr;
}

void SortedStruct::set_c2(const int32_t &value) {
  _flags[0] |= 256; _data_c2 = value;
}

uint32_t *SortedStruct::d2() {
  return _flags[0] & 512 ? &_data_d2 : nullptr;
}

const uint32_t *SortedStruct::d2() const {
  return _flags[0] & 512 ? &_data_d2 : nullptr;
}

void SortedStruct::set_d2(const uint32_t &value) {
  _flags[0] |= 512; _data_d2 = value;
}

float *SortedStruct::e2() {
  return _flags[0] & 1024 ? &_data_e2 : nullptr;
}

const float *SortedStruct::e2() const {
  return _flags[0] & 1024 ? &_data_e2 : nullptr;
}

void SortedStruct::set_e2(const float &value) {
  _flags[0] |= 1024; _data_e2 = value;
}

kiwi::String *SortedStruct::f2() {
  return _flags[0] & 2048 ? &_data_f2 : nullptr;
}

const kiwi::String *SortedStruct::f2() const {
  return _flags[0] & 2048 ? &_data_f2 : nullptr;
}

void SortedStruct::set_f2(const kiwi::String &value) {
  _flags[0] |= 2048; _data_f2 = value;
}

kiwi::Array<bool> *SortedStruct::a3() {
  return _flags[0] & 4096 ? &_data_a3 : nullptr;
}

const kiwi::Array<bool> *SortedStruct::a3() const {
  return _flags[0] & 4096 ? &_data_a3 : nullptr;
}

kiwi::Array<bool> &SortedStruct::set_a3(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 4096; return _data_a3 = pool.array<bool>(count);
}

kiwi::Array<uint8_t> *SortedStruct::b3() {
  return _flags[0] & 8192 ? &_data_b3 : nullptr;
}

const kiwi::Array<uint8_t> *SortedStruct::b3() const {
  return _flags[0] & 8192 ? &_data_b3 : nullptr;
}

kiwi::Array<uint8_t> &SortedStruct::set_b3(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 8192; return _data_b3 = pool.array<uint8_t>(count);
}

kiwi::Array<int32_t> *SortedStruct::c3() {
  return _flags[0] & 16384 ? &_data_c3 : nullptr;
}

const kiwi::Array<int32_t> *SortedStruct::c3() const {
  return _flags[0] & 16384 ? &_data_c3 : nullptr;
}

kiwi::Array<int32_t> &SortedStruct::set_c3(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 16384; return _data_c3 = pool.array<int32_t>(count);
}

kiwi::Array<uint32_t> *SortedStruct::d3() {
  return _flags[0] & 32768 ? &_data_d3 : nullptr;
}

const kiwi::Array<uint32_t> *SortedStruct::d3() const {
  return _flags[0] & 32768 ? &_data_d3 : nullptr;
}

kiwi::Array<uint32_t> &SortedStruct::set_d3(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 32768; return _data_d3 = pool.array<uint32_t>(count);
}

kiwi::Array<float> *SortedStruct::e3() {
  return _flags[0] & 65536 ? &_data_e3 : nullptr;
}

const kiwi::Array<float> *SortedStruct::e3() const {
  return _flags[0] & 65536 ? &_data_e3 : nullptr;
}

kiwi::Array<float> &SortedStruct::set_e3(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 65536; return _data_e3 = pool.array<float>(count);
}

kiwi::Array<kiwi::String> *SortedStruct::f3() {
  return _flags[0] & 131072 ? &_data_f3 : nullptr;
}

const kiwi::Array<kiwi::String> *SortedStruct::f3() const {
  return _flags[0] & 131072 ? &_data_f3 : nullptr;
}

kiwi::Array<kiwi::String> &SortedStruct::set_f3(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 131072; return _data_f3 = pool.array<kiwi::String>(count);
}

template <typename OutputByteBuffer> bool SortedStruct::encode(OutputByteBuffer &_bb) {
  if (a1() == nullptr) return false;
  _bb.writeByte(_data_a1);
  if (b1() == nullptr) return false;
  _bb.writeByte(_data_b1);
  if (c1() == nullptr) return false;
  _bb.writeVarInt(_data_c1);
  if (d1() == nullptr) return false;
  _bb.writeVarUint(_data_d1);
  if (e1() == nullptr) return false;
  _bb.writeVarFloat(_data_e1);
  if (f1() == nullptr) return false;
  _bb.writeString(_data_f1.c_str());
  if (a2() == nullptr) return false;
  _bb.writeByte(_data_a2);
  if (b2() == nullptr) return false;
  _bb.writeByte(_data_b2);
  if (c2() == nullptr) return false;
  _bb.writeVarInt(_data_c2);
  if (d2() == nullptr) return false;
  _bb.writeVarUint(_data_d2);
  if (e2() == nullptr) return false;
  _bb.writeVarFloat(_data_e2);
  if (f2() == nullptr) return false;
  _bb.writeString(_data_f2.c_str());
  if (a3() == nullptr) return false;
  _bb.writeVarUint(_data_a3.size());
  for (bool &_it : _data_a3) _bb.writeByte(_it);
  if (b3() == nullptr) return false;
  _bb.writeVarUint(_data_b3.size());
  for (uint8_t &_it : _data_b3) _bb.writeByte(_it);
  if (c3() == nullptr) return false;
  _bb.writeVarUint(_data_c3.size());
  for (int32_t &_it : _data_c3) _bb.writeVarInt(_it);
  if (d3() == nullptr) return false;
  _bb.writeVarUint(_data_d3.size());
  for (uint32_t &_it : _data_d3) _bb.writeVarUint(_it);
  if (e3() == nullptr) return false;
  _bb.writeVarUint(_data_e3.size());
  for (float &_it : _data_e3) _bb.writeVarFloat(_it);
  if (f3() == nullptr) return false;
  _bb.writeVarUint(_data_f3.size());
  for (kiwi::String &_it : _data_f3) _bb.writeString(_it.c_str());
  return true;
}

template <typename InputByteBuffer> bool SortedStruct::decode(InputByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  uint32_t _count;
  if (!_bb.readByte(_data_a1)) return false;
  set_a1(_data_a1);
  if (!_bb.readByte(_data_b1)) return false;
  set_b1(_data_b1);
  if (!_bb.readVarInt(_data_c1)) return false;
  set_c1(_data_c1);
  if (!_bb.readVarUint(_data_d1)) return false;
  set_d1(_data_d1);
  if (!_bb.readVarFloat(_data_e1)) return false;
  set_e1(_data_e1);
  if (!_bb.readString(_data_f1, _pool)) return false;
  set_f1(_data_f1);
  if (!_bb.readByte(_data_a2)) return false;
  set_a2(_data_a2);
  if (!_bb.readByte(_data_b2)) return false;
  set_b2(_data_b2);
  if (!_bb.readVarInt(_data_c2)) return false;
  set_c2(_data_c2);
  if (!_bb.readVarUint(_data_d2)) return false;
  set_d2(_data_d2);
  if (!_bb.readVarFloat(_data_e2)) return false;
  set_e2(_data_e2);
  if (!_bb.readString(_data_f2, _pool)) return false;
  set_f2(_data_f2);
  if (!_bb.readVarUint(_count)) return false;
  for (bool &_it : set_a3(_pool, _count)) if (!_bb.readByte(_it)) return false;
  if (!_bb.readVarUint(_count)) return false;
  for (uint8_t &_it : set_b3(_pool, _count)) if (!_bb.readByte(_it)) return false;
  if (!_bb.readVarUint(_count)) return false;
  for (int32_t &_it : set_c3(_pool, _count)) if (!_bb.readVarInt(_it)) return false;
  if (!_bb.readVarUint(_count)) return false;
  for (uint32_t &_it : set_d3(_pool, _count)) if (!_bb.readVarUint(_it)) return false;
  if (!_bb.readVarUint(_count)) return false;
  for (float &_it : set_e3(_pool, _count)) if (!_bb.readVarFloat(_it)) return false;
  if (!_bb.readVarUint(_count)) return false;
  for (kiwi::String &_it : set_f3(_pool, _count)) if (!_bb.readString(_it, _pool)) return false;
  return true;
}

#endif

}
