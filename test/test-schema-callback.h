#ifndef INCLUDE_TEST_H
#define INCLUDE_TEST_H

#include "kiwi.h"

namespace test {

enum class Enum : uint32_t {
  A = 100,
  B = 200,
};

class Visitor {
public:
  virtual void beginEnumStruct() = 0;
  virtual void visitEnumStruct_x(Enum x) = 0;
  virtual void visitEnumStruct_y_count(uint32_t size) = 0;
  virtual void visitEnumStruct_y_element(Enum y) = 0;
  virtual void endEnumStruct() = 0;
  virtual void visitBoolStruct(bool x) = 0;
  virtual void visitByteStruct(uint8_t x) = 0;
  virtual void visitIntStruct(int32_t x) = 0;
  virtual void visitUintStruct(uint32_t x) = 0;
  virtual void visitInt64Struct(int64_t x) = 0;
  virtual void visitUint64Struct(uint64_t x) = 0;
  virtual void visitFloatStruct(float x) = 0;
  virtual void visitStringStruct(const char *x) = 0;
  virtual void visitCompoundStruct(uint32_t x, uint32_t y) = 0;
  virtual void visitNestedStruct(uint32_t a, uint32_t b_x, uint32_t b_y, uint32_t c) = 0;
  virtual void beginBoolMessage() = 0;
  virtual void visitBoolMessage_x(bool x) = 0;
  virtual void endBoolMessage() = 0;
  virtual void beginByteMessage() = 0;
  virtual void visitByteMessage_x(uint8_t x) = 0;
  virtual void endByteMessage() = 0;
  virtual void beginIntMessage() = 0;
  virtual void visitIntMessage_x(int32_t x) = 0;
  virtual void endIntMessage() = 0;
  virtual void beginUintMessage() = 0;
  virtual void visitUintMessage_x(uint32_t x) = 0;
  virtual void endUintMessage() = 0;
  virtual void beginInt64Message() = 0;
  virtual void visitInt64Message_x(int64_t x) = 0;
  virtual void endInt64Message() = 0;
  virtual void beginUint64Message() = 0;
  virtual void visitUint64Message_x(uint64_t x) = 0;
  virtual void endUint64Message() = 0;
  virtual void beginFloatMessage() = 0;
  virtual void visitFloatMessage_x(float x) = 0;
  virtual void endFloatMessage() = 0;
  virtual void beginStringMessage() = 0;
  virtual void visitStringMessage_x(const char *x) = 0;
  virtual void endStringMessage() = 0;
  virtual void beginCompoundMessage() = 0;
  virtual void visitCompoundMessage_x(uint32_t x) = 0;
  virtual void visitCompoundMessage_y(uint32_t y) = 0;
  virtual void endCompoundMessage() = 0;
  virtual void beginNestedMessage() = 0;
  virtual void visitNestedMessage_a(uint32_t a) = 0;
  virtual void visitNestedMessage_b() = 0;
  virtual void visitNestedMessage_c(uint32_t c) = 0;
  virtual void endNestedMessage() = 0;
  virtual void beginBoolArrayStruct() = 0;
  virtual void visitBoolArrayStruct_x_count(uint32_t size) = 0;
  virtual void visitBoolArrayStruct_x_element(bool x) = 0;
  virtual void endBoolArrayStruct() = 0;
  virtual void beginByteArrayStruct() = 0;
  virtual void visitByteArrayStruct_x_count(uint32_t size) = 0;
  virtual void visitByteArrayStruct_x_element(uint8_t x) = 0;
  virtual void endByteArrayStruct() = 0;
  virtual void beginIntArrayStruct() = 0;
  virtual void visitIntArrayStruct_x_count(uint32_t size) = 0;
  virtual void visitIntArrayStruct_x_element(int32_t x) = 0;
  virtual void endIntArrayStruct() = 0;
  virtual void beginUintArrayStruct() = 0;
  virtual void visitUintArrayStruct_x_count(uint32_t size) = 0;
  virtual void visitUintArrayStruct_x_element(uint32_t x) = 0;
  virtual void endUintArrayStruct() = 0;
  virtual void beginInt64ArrayStruct() = 0;
  virtual void visitInt64ArrayStruct_x_count(uint32_t size) = 0;
  virtual void visitInt64ArrayStruct_x_element(int64_t x) = 0;
  virtual void endInt64ArrayStruct() = 0;
  virtual void beginUint64ArrayStruct() = 0;
  virtual void visitUint64ArrayStruct_x_count(uint32_t size) = 0;
  virtual void visitUint64ArrayStruct_x_element(uint64_t x) = 0;
  virtual void endUint64ArrayStruct() = 0;
  virtual void beginFloatArrayStruct() = 0;
  virtual void visitFloatArrayStruct_x_count(uint32_t size) = 0;
  virtual void visitFloatArrayStruct_x_element(float x) = 0;
  virtual void endFloatArrayStruct() = 0;
  virtual void beginStringArrayStruct() = 0;
  virtual void visitStringArrayStruct_x_count(uint32_t size) = 0;
  virtual void visitStringArrayStruct_x_element(const char *x) = 0;
  virtual void endStringArrayStruct() = 0;
  virtual void beginCompoundArrayStruct() = 0;
  virtual void visitCompoundArrayStruct_x_count(uint32_t size) = 0;
  virtual void visitCompoundArrayStruct_x_element(uint32_t x) = 0;
  virtual void visitCompoundArrayStruct_y_count(uint32_t size) = 0;
  virtual void visitCompoundArrayStruct_y_element(uint32_t y) = 0;
  virtual void endCompoundArrayStruct() = 0;
  virtual void beginBoolArrayMessage() = 0;
  virtual void visitBoolArrayMessage_x_count(uint32_t size) = 0;
  virtual void visitBoolArrayMessage_x_element(bool x) = 0;
  virtual void endBoolArrayMessage() = 0;
  virtual void beginByteArrayMessage() = 0;
  virtual void visitByteArrayMessage_x_count(uint32_t size) = 0;
  virtual void visitByteArrayMessage_x_element(uint8_t x) = 0;
  virtual void endByteArrayMessage() = 0;
  virtual void beginIntArrayMessage() = 0;
  virtual void visitIntArrayMessage_x_count(uint32_t size) = 0;
  virtual void visitIntArrayMessage_x_element(int32_t x) = 0;
  virtual void endIntArrayMessage() = 0;
  virtual void beginUintArrayMessage() = 0;
  virtual void visitUintArrayMessage_x_count(uint32_t size) = 0;
  virtual void visitUintArrayMessage_x_element(uint32_t x) = 0;
  virtual void endUintArrayMessage() = 0;
  virtual void beginInt64ArrayMessage() = 0;
  virtual void visitInt64ArrayMessage_x_count(uint32_t size) = 0;
  virtual void visitInt64ArrayMessage_x_element(int64_t x) = 0;
  virtual void endInt64ArrayMessage() = 0;
  virtual void beginUint64ArrayMessage() = 0;
  virtual void visitUint64ArrayMessage_x_count(uint32_t size) = 0;
  virtual void visitUint64ArrayMessage_x_element(uint64_t x) = 0;
  virtual void endUint64ArrayMessage() = 0;
  virtual void beginFloatArrayMessage() = 0;
  virtual void visitFloatArrayMessage_x_count(uint32_t size) = 0;
  virtual void visitFloatArrayMessage_x_element(float x) = 0;
  virtual void endFloatArrayMessage() = 0;
  virtual void beginStringArrayMessage() = 0;
  virtual void visitStringArrayMessage_x_count(uint32_t size) = 0;
  virtual void visitStringArrayMessage_x_element(const char *x) = 0;
  virtual void endStringArrayMessage() = 0;
  virtual void beginCompoundArrayMessage() = 0;
  virtual void visitCompoundArrayMessage_x_count(uint32_t size) = 0;
  virtual void visitCompoundArrayMessage_x_element(uint32_t x) = 0;
  virtual void visitCompoundArrayMessage_y_count(uint32_t size) = 0;
  virtual void visitCompoundArrayMessage_y_element(uint32_t y) = 0;
  virtual void endCompoundArrayMessage() = 0;
  virtual void beginRecursiveMessage() = 0;
  virtual void visitRecursiveMessage_x() = 0;
  virtual void endRecursiveMessage() = 0;
  virtual void beginNonDeprecatedMessage() = 0;
  virtual void visitNonDeprecatedMessage_a(uint32_t a) = 0;
  virtual void visitNonDeprecatedMessage_b(uint32_t b) = 0;
  virtual void visitNonDeprecatedMessage_c_count(uint32_t size) = 0;
  virtual void visitNonDeprecatedMessage_c_element(uint32_t c) = 0;
  virtual void visitNonDeprecatedMessage_d_count(uint32_t size) = 0;
  virtual void visitNonDeprecatedMessage_d_element(uint32_t d) = 0;
  virtual void visitNonDeprecatedMessage_e(uint8_t e_x) = 0;
  virtual void visitNonDeprecatedMessage_f(uint8_t f_x) = 0;
  virtual void visitNonDeprecatedMessage_g(uint32_t g) = 0;
  virtual void endNonDeprecatedMessage() = 0;
  virtual void beginDeprecatedMessage() = 0;
  virtual void visitDeprecatedMessage_a(uint32_t a) = 0;
  virtual void visitDeprecatedMessage_c_count(uint32_t size) = 0;
  virtual void visitDeprecatedMessage_c_element(uint32_t c) = 0;
  virtual void visitDeprecatedMessage_e(uint8_t e_x) = 0;
  virtual void visitDeprecatedMessage_g(uint32_t g) = 0;
  virtual void endDeprecatedMessage() = 0;
  virtual void beginSortedStruct() = 0;
  virtual void visitSortedStruct_a1(bool a1) = 0;
  virtual void visitSortedStruct_b1(uint8_t b1) = 0;
  virtual void visitSortedStruct_c1(int32_t c1) = 0;
  virtual void visitSortedStruct_d1(uint32_t d1) = 0;
  virtual void visitSortedStruct_e1(float e1) = 0;
  virtual void visitSortedStruct_f1(const char *f1) = 0;
  virtual void visitSortedStruct_g1(int64_t g1) = 0;
  virtual void visitSortedStruct_h1(uint64_t h1) = 0;
  virtual void visitSortedStruct_a2(bool a2) = 0;
  virtual void visitSortedStruct_b2(uint8_t b2) = 0;
  virtual void visitSortedStruct_c2(int32_t c2) = 0;
  virtual void visitSortedStruct_d2(uint32_t d2) = 0;
  virtual void visitSortedStruct_e2(float e2) = 0;
  virtual void visitSortedStruct_f2(const char *f2) = 0;
  virtual void visitSortedStruct_g2(int64_t g2) = 0;
  virtual void visitSortedStruct_h2(uint64_t h2) = 0;
  virtual void visitSortedStruct_a3_count(uint32_t size) = 0;
  virtual void visitSortedStruct_a3_element(bool a3) = 0;
  virtual void visitSortedStruct_b3_count(uint32_t size) = 0;
  virtual void visitSortedStruct_b3_element(uint8_t b3) = 0;
  virtual void visitSortedStruct_c3_count(uint32_t size) = 0;
  virtual void visitSortedStruct_c3_element(int32_t c3) = 0;
  virtual void visitSortedStruct_d3_count(uint32_t size) = 0;
  virtual void visitSortedStruct_d3_element(uint32_t d3) = 0;
  virtual void visitSortedStruct_e3_count(uint32_t size) = 0;
  virtual void visitSortedStruct_e3_element(float e3) = 0;
  virtual void visitSortedStruct_f3_count(uint32_t size) = 0;
  virtual void visitSortedStruct_f3_element(const char *f3) = 0;
  virtual void visitSortedStruct_g3_count(uint32_t size) = 0;
  virtual void visitSortedStruct_g3_element(int64_t g3) = 0;
  virtual void visitSortedStruct_h3_count(uint32_t size) = 0;
  virtual void visitSortedStruct_h3_element(uint64_t h3) = 0;
  virtual void endSortedStruct() = 0;
};

class Writer : public Visitor {
private:
  kiwi::ByteBuffer &_bb;
public:
  Writer(kiwi::ByteBuffer &bb) : _bb(bb) {}
  virtual void beginEnumStruct() override;
  virtual void visitEnumStruct_x(Enum x) override;
  virtual void visitEnumStruct_y_count(uint32_t size) override;
  virtual void visitEnumStruct_y_element(Enum y) override;
  virtual void endEnumStruct() override;
  virtual void visitBoolStruct(bool x) override;
  virtual void visitByteStruct(uint8_t x) override;
  virtual void visitIntStruct(int32_t x) override;
  virtual void visitUintStruct(uint32_t x) override;
  virtual void visitInt64Struct(int64_t x) override;
  virtual void visitUint64Struct(uint64_t x) override;
  virtual void visitFloatStruct(float x) override;
  virtual void visitStringStruct(const char *x) override;
  virtual void visitCompoundStruct(uint32_t x, uint32_t y) override;
  virtual void visitNestedStruct(uint32_t a, uint32_t b_x, uint32_t b_y, uint32_t c) override;
  virtual void beginBoolMessage() override;
  virtual void visitBoolMessage_x(bool x) override;
  virtual void endBoolMessage() override;
  virtual void beginByteMessage() override;
  virtual void visitByteMessage_x(uint8_t x) override;
  virtual void endByteMessage() override;
  virtual void beginIntMessage() override;
  virtual void visitIntMessage_x(int32_t x) override;
  virtual void endIntMessage() override;
  virtual void beginUintMessage() override;
  virtual void visitUintMessage_x(uint32_t x) override;
  virtual void endUintMessage() override;
  virtual void beginInt64Message() override;
  virtual void visitInt64Message_x(int64_t x) override;
  virtual void endInt64Message() override;
  virtual void beginUint64Message() override;
  virtual void visitUint64Message_x(uint64_t x) override;
  virtual void endUint64Message() override;
  virtual void beginFloatMessage() override;
  virtual void visitFloatMessage_x(float x) override;
  virtual void endFloatMessage() override;
  virtual void beginStringMessage() override;
  virtual void visitStringMessage_x(const char *x) override;
  virtual void endStringMessage() override;
  virtual void beginCompoundMessage() override;
  virtual void visitCompoundMessage_x(uint32_t x) override;
  virtual void visitCompoundMessage_y(uint32_t y) override;
  virtual void endCompoundMessage() override;
  virtual void beginNestedMessage() override;
  virtual void visitNestedMessage_a(uint32_t a) override;
  virtual void visitNestedMessage_b() override;
  virtual void visitNestedMessage_c(uint32_t c) override;
  virtual void endNestedMessage() override;
  virtual void beginBoolArrayStruct() override;
  virtual void visitBoolArrayStruct_x_count(uint32_t size) override;
  virtual void visitBoolArrayStruct_x_element(bool x) override;
  virtual void endBoolArrayStruct() override;
  virtual void beginByteArrayStruct() override;
  virtual void visitByteArrayStruct_x_count(uint32_t size) override;
  virtual void visitByteArrayStruct_x_element(uint8_t x) override;
  virtual void endByteArrayStruct() override;
  virtual void beginIntArrayStruct() override;
  virtual void visitIntArrayStruct_x_count(uint32_t size) override;
  virtual void visitIntArrayStruct_x_element(int32_t x) override;
  virtual void endIntArrayStruct() override;
  virtual void beginUintArrayStruct() override;
  virtual void visitUintArrayStruct_x_count(uint32_t size) override;
  virtual void visitUintArrayStruct_x_element(uint32_t x) override;
  virtual void endUintArrayStruct() override;
  virtual void beginInt64ArrayStruct() override;
  virtual void visitInt64ArrayStruct_x_count(uint32_t size) override;
  virtual void visitInt64ArrayStruct_x_element(int64_t x) override;
  virtual void endInt64ArrayStruct() override;
  virtual void beginUint64ArrayStruct() override;
  virtual void visitUint64ArrayStruct_x_count(uint32_t size) override;
  virtual void visitUint64ArrayStruct_x_element(uint64_t x) override;
  virtual void endUint64ArrayStruct() override;
  virtual void beginFloatArrayStruct() override;
  virtual void visitFloatArrayStruct_x_count(uint32_t size) override;
  virtual void visitFloatArrayStruct_x_element(float x) override;
  virtual void endFloatArrayStruct() override;
  virtual void beginStringArrayStruct() override;
  virtual void visitStringArrayStruct_x_count(uint32_t size) override;
  virtual void visitStringArrayStruct_x_element(const char *x) override;
  virtual void endStringArrayStruct() override;
  virtual void beginCompoundArrayStruct() override;
  virtual void visitCompoundArrayStruct_x_count(uint32_t size) override;
  virtual void visitCompoundArrayStruct_x_element(uint32_t x) override;
  virtual void visitCompoundArrayStruct_y_count(uint32_t size) override;
  virtual void visitCompoundArrayStruct_y_element(uint32_t y) override;
  virtual void endCompoundArrayStruct() override;
  virtual void beginBoolArrayMessage() override;
  virtual void visitBoolArrayMessage_x_count(uint32_t size) override;
  virtual void visitBoolArrayMessage_x_element(bool x) override;
  virtual void endBoolArrayMessage() override;
  virtual void beginByteArrayMessage() override;
  virtual void visitByteArrayMessage_x_count(uint32_t size) override;
  virtual void visitByteArrayMessage_x_element(uint8_t x) override;
  virtual void endByteArrayMessage() override;
  virtual void beginIntArrayMessage() override;
  virtual void visitIntArrayMessage_x_count(uint32_t size) override;
  virtual void visitIntArrayMessage_x_element(int32_t x) override;
  virtual void endIntArrayMessage() override;
  virtual void beginUintArrayMessage() override;
  virtual void visitUintArrayMessage_x_count(uint32_t size) override;
  virtual void visitUintArrayMessage_x_element(uint32_t x) override;
  virtual void endUintArrayMessage() override;
  virtual void beginInt64ArrayMessage() override;
  virtual void visitInt64ArrayMessage_x_count(uint32_t size) override;
  virtual void visitInt64ArrayMessage_x_element(int64_t x) override;
  virtual void endInt64ArrayMessage() override;
  virtual void beginUint64ArrayMessage() override;
  virtual void visitUint64ArrayMessage_x_count(uint32_t size) override;
  virtual void visitUint64ArrayMessage_x_element(uint64_t x) override;
  virtual void endUint64ArrayMessage() override;
  virtual void beginFloatArrayMessage() override;
  virtual void visitFloatArrayMessage_x_count(uint32_t size) override;
  virtual void visitFloatArrayMessage_x_element(float x) override;
  virtual void endFloatArrayMessage() override;
  virtual void beginStringArrayMessage() override;
  virtual void visitStringArrayMessage_x_count(uint32_t size) override;
  virtual void visitStringArrayMessage_x_element(const char *x) override;
  virtual void endStringArrayMessage() override;
  virtual void beginCompoundArrayMessage() override;
  virtual void visitCompoundArrayMessage_x_count(uint32_t size) override;
  virtual void visitCompoundArrayMessage_x_element(uint32_t x) override;
  virtual void visitCompoundArrayMessage_y_count(uint32_t size) override;
  virtual void visitCompoundArrayMessage_y_element(uint32_t y) override;
  virtual void endCompoundArrayMessage() override;
  virtual void beginRecursiveMessage() override;
  virtual void visitRecursiveMessage_x() override;
  virtual void endRecursiveMessage() override;
  virtual void beginNonDeprecatedMessage() override;
  virtual void visitNonDeprecatedMessage_a(uint32_t a) override;
  virtual void visitNonDeprecatedMessage_b(uint32_t b) override;
  virtual void visitNonDeprecatedMessage_c_count(uint32_t size) override;
  virtual void visitNonDeprecatedMessage_c_element(uint32_t c) override;
  virtual void visitNonDeprecatedMessage_d_count(uint32_t size) override;
  virtual void visitNonDeprecatedMessage_d_element(uint32_t d) override;
  virtual void visitNonDeprecatedMessage_e(uint8_t e_x) override;
  virtual void visitNonDeprecatedMessage_f(uint8_t f_x) override;
  virtual void visitNonDeprecatedMessage_g(uint32_t g) override;
  virtual void endNonDeprecatedMessage() override;
  virtual void beginDeprecatedMessage() override;
  virtual void visitDeprecatedMessage_a(uint32_t a) override;
  virtual void visitDeprecatedMessage_c_count(uint32_t size) override;
  virtual void visitDeprecatedMessage_c_element(uint32_t c) override;
  virtual void visitDeprecatedMessage_e(uint8_t e_x) override;
  virtual void visitDeprecatedMessage_g(uint32_t g) override;
  virtual void endDeprecatedMessage() override;
  virtual void beginSortedStruct() override;
  virtual void visitSortedStruct_a1(bool a1) override;
  virtual void visitSortedStruct_b1(uint8_t b1) override;
  virtual void visitSortedStruct_c1(int32_t c1) override;
  virtual void visitSortedStruct_d1(uint32_t d1) override;
  virtual void visitSortedStruct_e1(float e1) override;
  virtual void visitSortedStruct_f1(const char *f1) override;
  virtual void visitSortedStruct_g1(int64_t g1) override;
  virtual void visitSortedStruct_h1(uint64_t h1) override;
  virtual void visitSortedStruct_a2(bool a2) override;
  virtual void visitSortedStruct_b2(uint8_t b2) override;
  virtual void visitSortedStruct_c2(int32_t c2) override;
  virtual void visitSortedStruct_d2(uint32_t d2) override;
  virtual void visitSortedStruct_e2(float e2) override;
  virtual void visitSortedStruct_f2(const char *f2) override;
  virtual void visitSortedStruct_g2(int64_t g2) override;
  virtual void visitSortedStruct_h2(uint64_t h2) override;
  virtual void visitSortedStruct_a3_count(uint32_t size) override;
  virtual void visitSortedStruct_a3_element(bool a3) override;
  virtual void visitSortedStruct_b3_count(uint32_t size) override;
  virtual void visitSortedStruct_b3_element(uint8_t b3) override;
  virtual void visitSortedStruct_c3_count(uint32_t size) override;
  virtual void visitSortedStruct_c3_element(int32_t c3) override;
  virtual void visitSortedStruct_d3_count(uint32_t size) override;
  virtual void visitSortedStruct_d3_element(uint32_t d3) override;
  virtual void visitSortedStruct_e3_count(uint32_t size) override;
  virtual void visitSortedStruct_e3_element(float e3) override;
  virtual void visitSortedStruct_f3_count(uint32_t size) override;
  virtual void visitSortedStruct_f3_element(const char *f3) override;
  virtual void visitSortedStruct_g3_count(uint32_t size) override;
  virtual void visitSortedStruct_g3_element(int64_t g3) override;
  virtual void visitSortedStruct_h3_count(uint32_t size) override;
  virtual void visitSortedStruct_h3_element(uint64_t h3) override;
  virtual void endSortedStruct() override;
};

bool parseEnumStruct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseBoolStruct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseByteStruct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseIntStruct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseUintStruct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseInt64Struct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseUint64Struct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseFloatStruct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseStringStruct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseCompoundStruct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseNestedStruct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseBoolMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseByteMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseIntMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseUintMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseInt64Message(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseUint64Message(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseFloatMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseStringMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseCompoundMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseNestedMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseBoolArrayStruct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseByteArrayStruct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseIntArrayStruct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseUintArrayStruct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseInt64ArrayStruct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseUint64ArrayStruct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseFloatArrayStruct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseStringArrayStruct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseCompoundArrayStruct(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseBoolArrayMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseByteArrayMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseIntArrayMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseUintArrayMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseInt64ArrayMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseUint64ArrayMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseFloatArrayMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseStringArrayMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseCompoundArrayMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseRecursiveMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseNonDeprecatedMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseDeprecatedMessage(kiwi::ByteBuffer &bb, Visitor &visitor);
bool parseSortedStruct(kiwi::ByteBuffer &bb, Visitor &visitor);

#ifdef IMPLEMENT_SCHEMA_H

bool parseEnumStruct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginEnumStruct();
  Enum x;
  if (!bb.readVarUint(reinterpret_cast<uint32_t &>(x))) return false;
  visitor.visitEnumStruct_x(x);
  uint32_t _y_count;
  if (!bb.readVarUint(_y_count)) return false;
  visitor.visitEnumStruct_y_count(_y_count);
  while (_y_count-- > 0) {
    Enum y;
    if (!bb.readVarUint(reinterpret_cast<uint32_t &>(y))) return false;
    visitor.visitEnumStruct_y_element(y);
  }
  visitor.endEnumStruct();
  return true;
}

bool parseBoolStruct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  bool x;
  if (!bb.readByte(x)) return false;
  visitor.visitBoolStruct(x);
  return true;
}

bool parseByteStruct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  uint8_t x;
  if (!bb.readByte(x)) return false;
  visitor.visitByteStruct(x);
  return true;
}

bool parseIntStruct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  int32_t x;
  if (!bb.readVarInt(x)) return false;
  visitor.visitIntStruct(x);
  return true;
}

bool parseUintStruct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  uint32_t x;
  if (!bb.readVarUint(x)) return false;
  visitor.visitUintStruct(x);
  return true;
}

bool parseInt64Struct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  int64_t x;
  if (!bb.readVarInt64(x)) return false;
  visitor.visitInt64Struct(x);
  return true;
}

bool parseUint64Struct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  uint64_t x;
  if (!bb.readVarUint64(x)) return false;
  visitor.visitUint64Struct(x);
  return true;
}

bool parseFloatStruct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  float x;
  if (!bb.readVarFloat(x)) return false;
  visitor.visitFloatStruct(x);
  return true;
}

bool parseStringStruct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  const char *x;
  if (!bb.readString(x)) return false;
  visitor.visitStringStruct(x);
  return true;
}

bool parseCompoundStruct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  uint32_t x;
  uint32_t y;
  if (!bb.readVarUint(x) || !bb.readVarUint(y)) return false;
  visitor.visitCompoundStruct(x, y);
  return true;
}

bool parseNestedStruct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  uint32_t a;
  uint32_t b_x;
  uint32_t b_y;
  uint32_t c;
  if (!bb.readVarUint(a) || !bb.readVarUint(b_x) || !bb.readVarUint(b_y) || !bb.readVarUint(c)) return false;
  visitor.visitNestedStruct(a, b_x, b_y, c);
  return true;
}

bool parseBoolMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginBoolMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endBoolMessage();
        return true;
      }
      case 1: {
        bool x;
        if (!bb.readByte(x)) return false;
        visitor.visitBoolMessage_x(x);
        break;
      }
      default: return false;
    }
  }
}

bool parseByteMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginByteMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endByteMessage();
        return true;
      }
      case 1: {
        uint8_t x;
        if (!bb.readByte(x)) return false;
        visitor.visitByteMessage_x(x);
        break;
      }
      default: return false;
    }
  }
}

bool parseIntMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginIntMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endIntMessage();
        return true;
      }
      case 1: {
        int32_t x;
        if (!bb.readVarInt(x)) return false;
        visitor.visitIntMessage_x(x);
        break;
      }
      default: return false;
    }
  }
}

bool parseUintMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginUintMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endUintMessage();
        return true;
      }
      case 1: {
        uint32_t x;
        if (!bb.readVarUint(x)) return false;
        visitor.visitUintMessage_x(x);
        break;
      }
      default: return false;
    }
  }
}

bool parseInt64Message(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginInt64Message();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endInt64Message();
        return true;
      }
      case 1: {
        int64_t x;
        if (!bb.readVarInt64(x)) return false;
        visitor.visitInt64Message_x(x);
        break;
      }
      default: return false;
    }
  }
}

bool parseUint64Message(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginUint64Message();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endUint64Message();
        return true;
      }
      case 1: {
        uint64_t x;
        if (!bb.readVarUint64(x)) return false;
        visitor.visitUint64Message_x(x);
        break;
      }
      default: return false;
    }
  }
}

bool parseFloatMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginFloatMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endFloatMessage();
        return true;
      }
      case 1: {
        float x;
        if (!bb.readVarFloat(x)) return false;
        visitor.visitFloatMessage_x(x);
        break;
      }
      default: return false;
    }
  }
}

bool parseStringMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginStringMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endStringMessage();
        return true;
      }
      case 1: {
        const char *x;
        if (!bb.readString(x)) return false;
        visitor.visitStringMessage_x(x);
        break;
      }
      default: return false;
    }
  }
}

bool parseCompoundMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginCompoundMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endCompoundMessage();
        return true;
      }
      case 1: {
        uint32_t x;
        if (!bb.readVarUint(x)) return false;
        visitor.visitCompoundMessage_x(x);
        break;
      }
      case 2: {
        uint32_t y;
        if (!bb.readVarUint(y)) return false;
        visitor.visitCompoundMessage_y(y);
        break;
      }
      default: return false;
    }
  }
}

bool parseNestedMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginNestedMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endNestedMessage();
        return true;
      }
      case 1: {
        uint32_t a;
        if (!bb.readVarUint(a)) return false;
        visitor.visitNestedMessage_a(a);
        break;
      }
      case 2: {
        visitor.visitNestedMessage_b();
        if (!parseCompoundMessage(bb, visitor)) return false;
        break;
      }
      case 3: {
        uint32_t c;
        if (!bb.readVarUint(c)) return false;
        visitor.visitNestedMessage_c(c);
        break;
      }
      default: return false;
    }
  }
}

bool parseBoolArrayStruct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginBoolArrayStruct();
  uint32_t _x_count;
  if (!bb.readVarUint(_x_count)) return false;
  visitor.visitBoolArrayStruct_x_count(_x_count);
  while (_x_count-- > 0) {
    bool x;
    if (!bb.readByte(x)) return false;
    visitor.visitBoolArrayStruct_x_element(x);
  }
  visitor.endBoolArrayStruct();
  return true;
}

bool parseByteArrayStruct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginByteArrayStruct();
  uint32_t _x_count;
  if (!bb.readVarUint(_x_count)) return false;
  visitor.visitByteArrayStruct_x_count(_x_count);
  while (_x_count-- > 0) {
    uint8_t x;
    if (!bb.readByte(x)) return false;
    visitor.visitByteArrayStruct_x_element(x);
  }
  visitor.endByteArrayStruct();
  return true;
}

bool parseIntArrayStruct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginIntArrayStruct();
  uint32_t _x_count;
  if (!bb.readVarUint(_x_count)) return false;
  visitor.visitIntArrayStruct_x_count(_x_count);
  while (_x_count-- > 0) {
    int32_t x;
    if (!bb.readVarInt(x)) return false;
    visitor.visitIntArrayStruct_x_element(x);
  }
  visitor.endIntArrayStruct();
  return true;
}

bool parseUintArrayStruct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginUintArrayStruct();
  uint32_t _x_count;
  if (!bb.readVarUint(_x_count)) return false;
  visitor.visitUintArrayStruct_x_count(_x_count);
  while (_x_count-- > 0) {
    uint32_t x;
    if (!bb.readVarUint(x)) return false;
    visitor.visitUintArrayStruct_x_element(x);
  }
  visitor.endUintArrayStruct();
  return true;
}

bool parseInt64ArrayStruct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginInt64ArrayStruct();
  uint32_t _x_count;
  if (!bb.readVarUint(_x_count)) return false;
  visitor.visitInt64ArrayStruct_x_count(_x_count);
  while (_x_count-- > 0) {
    int64_t x;
    if (!bb.readVarInt64(x)) return false;
    visitor.visitInt64ArrayStruct_x_element(x);
  }
  visitor.endInt64ArrayStruct();
  return true;
}

bool parseUint64ArrayStruct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginUint64ArrayStruct();
  uint32_t _x_count;
  if (!bb.readVarUint(_x_count)) return false;
  visitor.visitUint64ArrayStruct_x_count(_x_count);
  while (_x_count-- > 0) {
    uint64_t x;
    if (!bb.readVarUint64(x)) return false;
    visitor.visitUint64ArrayStruct_x_element(x);
  }
  visitor.endUint64ArrayStruct();
  return true;
}

bool parseFloatArrayStruct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginFloatArrayStruct();
  uint32_t _x_count;
  if (!bb.readVarUint(_x_count)) return false;
  visitor.visitFloatArrayStruct_x_count(_x_count);
  while (_x_count-- > 0) {
    float x;
    if (!bb.readVarFloat(x)) return false;
    visitor.visitFloatArrayStruct_x_element(x);
  }
  visitor.endFloatArrayStruct();
  return true;
}

bool parseStringArrayStruct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginStringArrayStruct();
  uint32_t _x_count;
  if (!bb.readVarUint(_x_count)) return false;
  visitor.visitStringArrayStruct_x_count(_x_count);
  while (_x_count-- > 0) {
    const char *x;
    if (!bb.readString(x)) return false;
    visitor.visitStringArrayStruct_x_element(x);
  }
  visitor.endStringArrayStruct();
  return true;
}

bool parseCompoundArrayStruct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginCompoundArrayStruct();
  uint32_t _x_count;
  if (!bb.readVarUint(_x_count)) return false;
  visitor.visitCompoundArrayStruct_x_count(_x_count);
  while (_x_count-- > 0) {
    uint32_t x;
    if (!bb.readVarUint(x)) return false;
    visitor.visitCompoundArrayStruct_x_element(x);
  }
  uint32_t _y_count;
  if (!bb.readVarUint(_y_count)) return false;
  visitor.visitCompoundArrayStruct_y_count(_y_count);
  while (_y_count-- > 0) {
    uint32_t y;
    if (!bb.readVarUint(y)) return false;
    visitor.visitCompoundArrayStruct_y_element(y);
  }
  visitor.endCompoundArrayStruct();
  return true;
}

bool parseBoolArrayMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginBoolArrayMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endBoolArrayMessage();
        return true;
      }
      case 1: {
        uint32_t _x_count;
        if (!bb.readVarUint(_x_count)) return false;
        visitor.visitBoolArrayMessage_x_count(_x_count);
        while (_x_count-- > 0) {
          bool x;
          if (!bb.readByte(x)) return false;
          visitor.visitBoolArrayMessage_x_element(x);
        }
        break;
      }
      default: return false;
    }
  }
}

bool parseByteArrayMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginByteArrayMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endByteArrayMessage();
        return true;
      }
      case 1: {
        uint32_t _x_count;
        if (!bb.readVarUint(_x_count)) return false;
        visitor.visitByteArrayMessage_x_count(_x_count);
        while (_x_count-- > 0) {
          uint8_t x;
          if (!bb.readByte(x)) return false;
          visitor.visitByteArrayMessage_x_element(x);
        }
        break;
      }
      default: return false;
    }
  }
}

bool parseIntArrayMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginIntArrayMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endIntArrayMessage();
        return true;
      }
      case 1: {
        uint32_t _x_count;
        if (!bb.readVarUint(_x_count)) return false;
        visitor.visitIntArrayMessage_x_count(_x_count);
        while (_x_count-- > 0) {
          int32_t x;
          if (!bb.readVarInt(x)) return false;
          visitor.visitIntArrayMessage_x_element(x);
        }
        break;
      }
      default: return false;
    }
  }
}

bool parseUintArrayMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginUintArrayMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endUintArrayMessage();
        return true;
      }
      case 1: {
        uint32_t _x_count;
        if (!bb.readVarUint(_x_count)) return false;
        visitor.visitUintArrayMessage_x_count(_x_count);
        while (_x_count-- > 0) {
          uint32_t x;
          if (!bb.readVarUint(x)) return false;
          visitor.visitUintArrayMessage_x_element(x);
        }
        break;
      }
      default: return false;
    }
  }
}

bool parseInt64ArrayMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginInt64ArrayMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endInt64ArrayMessage();
        return true;
      }
      case 1: {
        uint32_t _x_count;
        if (!bb.readVarUint(_x_count)) return false;
        visitor.visitInt64ArrayMessage_x_count(_x_count);
        while (_x_count-- > 0) {
          int64_t x;
          if (!bb.readVarInt64(x)) return false;
          visitor.visitInt64ArrayMessage_x_element(x);
        }
        break;
      }
      default: return false;
    }
  }
}

bool parseUint64ArrayMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginUint64ArrayMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endUint64ArrayMessage();
        return true;
      }
      case 1: {
        uint32_t _x_count;
        if (!bb.readVarUint(_x_count)) return false;
        visitor.visitUint64ArrayMessage_x_count(_x_count);
        while (_x_count-- > 0) {
          uint64_t x;
          if (!bb.readVarUint64(x)) return false;
          visitor.visitUint64ArrayMessage_x_element(x);
        }
        break;
      }
      default: return false;
    }
  }
}

bool parseFloatArrayMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginFloatArrayMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endFloatArrayMessage();
        return true;
      }
      case 1: {
        uint32_t _x_count;
        if (!bb.readVarUint(_x_count)) return false;
        visitor.visitFloatArrayMessage_x_count(_x_count);
        while (_x_count-- > 0) {
          float x;
          if (!bb.readVarFloat(x)) return false;
          visitor.visitFloatArrayMessage_x_element(x);
        }
        break;
      }
      default: return false;
    }
  }
}

bool parseStringArrayMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginStringArrayMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endStringArrayMessage();
        return true;
      }
      case 1: {
        uint32_t _x_count;
        if (!bb.readVarUint(_x_count)) return false;
        visitor.visitStringArrayMessage_x_count(_x_count);
        while (_x_count-- > 0) {
          const char *x;
          if (!bb.readString(x)) return false;
          visitor.visitStringArrayMessage_x_element(x);
        }
        break;
      }
      default: return false;
    }
  }
}

bool parseCompoundArrayMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginCompoundArrayMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endCompoundArrayMessage();
        return true;
      }
      case 1: {
        uint32_t _x_count;
        if (!bb.readVarUint(_x_count)) return false;
        visitor.visitCompoundArrayMessage_x_count(_x_count);
        while (_x_count-- > 0) {
          uint32_t x;
          if (!bb.readVarUint(x)) return false;
          visitor.visitCompoundArrayMessage_x_element(x);
        }
        break;
      }
      case 2: {
        uint32_t _y_count;
        if (!bb.readVarUint(_y_count)) return false;
        visitor.visitCompoundArrayMessage_y_count(_y_count);
        while (_y_count-- > 0) {
          uint32_t y;
          if (!bb.readVarUint(y)) return false;
          visitor.visitCompoundArrayMessage_y_element(y);
        }
        break;
      }
      default: return false;
    }
  }
}

bool parseRecursiveMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginRecursiveMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endRecursiveMessage();
        return true;
      }
      case 1: {
        visitor.visitRecursiveMessage_x();
        if (!parseRecursiveMessage(bb, visitor)) return false;
        break;
      }
      default: return false;
    }
  }
}

bool parseNonDeprecatedMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginNonDeprecatedMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endNonDeprecatedMessage();
        return true;
      }
      case 1: {
        uint32_t a;
        if (!bb.readVarUint(a)) return false;
        visitor.visitNonDeprecatedMessage_a(a);
        break;
      }
      case 2: {
        uint32_t b;
        if (!bb.readVarUint(b)) return false;
        visitor.visitNonDeprecatedMessage_b(b);
        break;
      }
      case 3: {
        uint32_t _c_count;
        if (!bb.readVarUint(_c_count)) return false;
        visitor.visitNonDeprecatedMessage_c_count(_c_count);
        while (_c_count-- > 0) {
          uint32_t c;
          if (!bb.readVarUint(c)) return false;
          visitor.visitNonDeprecatedMessage_c_element(c);
        }
        break;
      }
      case 4: {
        uint32_t _d_count;
        if (!bb.readVarUint(_d_count)) return false;
        visitor.visitNonDeprecatedMessage_d_count(_d_count);
        while (_d_count-- > 0) {
          uint32_t d;
          if (!bb.readVarUint(d)) return false;
          visitor.visitNonDeprecatedMessage_d_element(d);
        }
        break;
      }
      case 5: {
        uint8_t e_x;
        if (!bb.readByte(e_x)) return false;
        visitor.visitNonDeprecatedMessage_e(e_x);
        break;
      }
      case 6: {
        uint8_t f_x;
        if (!bb.readByte(f_x)) return false;
        visitor.visitNonDeprecatedMessage_f(f_x);
        break;
      }
      case 7: {
        uint32_t g;
        if (!bb.readVarUint(g)) return false;
        visitor.visitNonDeprecatedMessage_g(g);
        break;
      }
      default: return false;
    }
  }
}

bool parseDeprecatedMessage(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginDeprecatedMessage();
  while (true) {
    uint32_t _type;
    if (!bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0: {
        visitor.endDeprecatedMessage();
        return true;
      }
      case 1: {
        uint32_t a;
        if (!bb.readVarUint(a)) return false;
        visitor.visitDeprecatedMessage_a(a);
        break;
      }
      case 2: {
        uint32_t b;
        if (!bb.readVarUint(b)) return false;
        break;
      }
      case 3: {
        uint32_t _c_count;
        if (!bb.readVarUint(_c_count)) return false;
        visitor.visitDeprecatedMessage_c_count(_c_count);
        while (_c_count-- > 0) {
          uint32_t c;
          if (!bb.readVarUint(c)) return false;
          visitor.visitDeprecatedMessage_c_element(c);
        }
        break;
      }
      case 4: {
        uint32_t _d_count;
        if (!bb.readVarUint(_d_count)) return false;
        while (_d_count-- > 0) {
          uint32_t d;
          if (!bb.readVarUint(d)) return false;
        }
        break;
      }
      case 5: {
        uint8_t e_x;
        if (!bb.readByte(e_x)) return false;
        visitor.visitDeprecatedMessage_e(e_x);
        break;
      }
      case 6: {
        uint8_t f_x;
        if (!bb.readByte(f_x)) return false;
        break;
      }
      case 7: {
        uint32_t g;
        if (!bb.readVarUint(g)) return false;
        visitor.visitDeprecatedMessage_g(g);
        break;
      }
      default: return false;
    }
  }
}

bool parseSortedStruct(kiwi::ByteBuffer &bb, Visitor &visitor) {
  visitor.beginSortedStruct();
  bool a1;
  if (!bb.readByte(a1)) return false;
  visitor.visitSortedStruct_a1(a1);
  uint8_t b1;
  if (!bb.readByte(b1)) return false;
  visitor.visitSortedStruct_b1(b1);
  int32_t c1;
  if (!bb.readVarInt(c1)) return false;
  visitor.visitSortedStruct_c1(c1);
  uint32_t d1;
  if (!bb.readVarUint(d1)) return false;
  visitor.visitSortedStruct_d1(d1);
  float e1;
  if (!bb.readVarFloat(e1)) return false;
  visitor.visitSortedStruct_e1(e1);
  const char *f1;
  if (!bb.readString(f1)) return false;
  visitor.visitSortedStruct_f1(f1);
  int64_t g1;
  if (!bb.readVarInt64(g1)) return false;
  visitor.visitSortedStruct_g1(g1);
  uint64_t h1;
  if (!bb.readVarUint64(h1)) return false;
  visitor.visitSortedStruct_h1(h1);
  bool a2;
  if (!bb.readByte(a2)) return false;
  visitor.visitSortedStruct_a2(a2);
  uint8_t b2;
  if (!bb.readByte(b2)) return false;
  visitor.visitSortedStruct_b2(b2);
  int32_t c2;
  if (!bb.readVarInt(c2)) return false;
  visitor.visitSortedStruct_c2(c2);
  uint32_t d2;
  if (!bb.readVarUint(d2)) return false;
  visitor.visitSortedStruct_d2(d2);
  float e2;
  if (!bb.readVarFloat(e2)) return false;
  visitor.visitSortedStruct_e2(e2);
  const char *f2;
  if (!bb.readString(f2)) return false;
  visitor.visitSortedStruct_f2(f2);
  int64_t g2;
  if (!bb.readVarInt64(g2)) return false;
  visitor.visitSortedStruct_g2(g2);
  uint64_t h2;
  if (!bb.readVarUint64(h2)) return false;
  visitor.visitSortedStruct_h2(h2);
  uint32_t _a3_count;
  if (!bb.readVarUint(_a3_count)) return false;
  visitor.visitSortedStruct_a3_count(_a3_count);
  while (_a3_count-- > 0) {
    bool a3;
    if (!bb.readByte(a3)) return false;
    visitor.visitSortedStruct_a3_element(a3);
  }
  uint32_t _b3_count;
  if (!bb.readVarUint(_b3_count)) return false;
  visitor.visitSortedStruct_b3_count(_b3_count);
  while (_b3_count-- > 0) {
    uint8_t b3;
    if (!bb.readByte(b3)) return false;
    visitor.visitSortedStruct_b3_element(b3);
  }
  uint32_t _c3_count;
  if (!bb.readVarUint(_c3_count)) return false;
  visitor.visitSortedStruct_c3_count(_c3_count);
  while (_c3_count-- > 0) {
    int32_t c3;
    if (!bb.readVarInt(c3)) return false;
    visitor.visitSortedStruct_c3_element(c3);
  }
  uint32_t _d3_count;
  if (!bb.readVarUint(_d3_count)) return false;
  visitor.visitSortedStruct_d3_count(_d3_count);
  while (_d3_count-- > 0) {
    uint32_t d3;
    if (!bb.readVarUint(d3)) return false;
    visitor.visitSortedStruct_d3_element(d3);
  }
  uint32_t _e3_count;
  if (!bb.readVarUint(_e3_count)) return false;
  visitor.visitSortedStruct_e3_count(_e3_count);
  while (_e3_count-- > 0) {
    float e3;
    if (!bb.readVarFloat(e3)) return false;
    visitor.visitSortedStruct_e3_element(e3);
  }
  uint32_t _f3_count;
  if (!bb.readVarUint(_f3_count)) return false;
  visitor.visitSortedStruct_f3_count(_f3_count);
  while (_f3_count-- > 0) {
    const char *f3;
    if (!bb.readString(f3)) return false;
    visitor.visitSortedStruct_f3_element(f3);
  }
  uint32_t _g3_count;
  if (!bb.readVarUint(_g3_count)) return false;
  visitor.visitSortedStruct_g3_count(_g3_count);
  while (_g3_count-- > 0) {
    int64_t g3;
    if (!bb.readVarInt64(g3)) return false;
    visitor.visitSortedStruct_g3_element(g3);
  }
  uint32_t _h3_count;
  if (!bb.readVarUint(_h3_count)) return false;
  visitor.visitSortedStruct_h3_count(_h3_count);
  while (_h3_count-- > 0) {
    uint64_t h3;
    if (!bb.readVarUint64(h3)) return false;
    visitor.visitSortedStruct_h3_element(h3);
  }
  visitor.endSortedStruct();
  return true;
}

void Writer::beginEnumStruct() {
}

void Writer::visitEnumStruct_x(Enum x) {
  _bb.writeVarUint(static_cast<uint32_t>(x));
}

void Writer::visitEnumStruct_y_count(uint32_t size) {
  _bb.writeVarUint(size);
}

void Writer::visitEnumStruct_y_element(Enum y) {
  _bb.writeVarUint(static_cast<uint32_t>(y));
}

void Writer::endEnumStruct() {
}

void Writer::visitBoolStruct(bool x) {
  _bb.writeByte(x);
}

void Writer::visitByteStruct(uint8_t x) {
  _bb.writeByte(x);
}

void Writer::visitIntStruct(int32_t x) {
  _bb.writeVarInt(x);
}

void Writer::visitUintStruct(uint32_t x) {
  _bb.writeVarUint(x);
}

void Writer::visitInt64Struct(int64_t x) {
  _bb.writeVarInt64(x);
}

void Writer::visitUint64Struct(uint64_t x) {
  _bb.writeVarUint64(x);
}

void Writer::visitFloatStruct(float x) {
  _bb.writeVarFloat(x);
}

void Writer::visitStringStruct(const char *x) {
  _bb.writeString(x);
}

void Writer::visitCompoundStruct(uint32_t x, uint32_t y) {
  _bb.writeVarUint(x);
  _bb.writeVarUint(y);
}

void Writer::visitNestedStruct(uint32_t a, uint32_t b_x, uint32_t b_y, uint32_t c) {
  _bb.writeVarUint(a);
  _bb.writeVarUint(b_x);
  _bb.writeVarUint(b_y);
  _bb.writeVarUint(c);
}

void Writer::beginBoolMessage() {
}

void Writer::visitBoolMessage_x(bool x) {
  _bb.writeVarUint(1);
  _bb.writeByte(x);
}

void Writer::endBoolMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginByteMessage() {
}

void Writer::visitByteMessage_x(uint8_t x) {
  _bb.writeVarUint(1);
  _bb.writeByte(x);
}

void Writer::endByteMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginIntMessage() {
}

void Writer::visitIntMessage_x(int32_t x) {
  _bb.writeVarUint(1);
  _bb.writeVarInt(x);
}

void Writer::endIntMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginUintMessage() {
}

void Writer::visitUintMessage_x(uint32_t x) {
  _bb.writeVarUint(1);
  _bb.writeVarUint(x);
}

void Writer::endUintMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginInt64Message() {
}

void Writer::visitInt64Message_x(int64_t x) {
  _bb.writeVarUint(1);
  _bb.writeVarInt64(x);
}

void Writer::endInt64Message() {
  _bb.writeVarUint(0);
}

void Writer::beginUint64Message() {
}

void Writer::visitUint64Message_x(uint64_t x) {
  _bb.writeVarUint(1);
  _bb.writeVarUint64(x);
}

void Writer::endUint64Message() {
  _bb.writeVarUint(0);
}

void Writer::beginFloatMessage() {
}

void Writer::visitFloatMessage_x(float x) {
  _bb.writeVarUint(1);
  _bb.writeVarFloat(x);
}

void Writer::endFloatMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginStringMessage() {
}

void Writer::visitStringMessage_x(const char *x) {
  _bb.writeVarUint(1);
  _bb.writeString(x);
}

void Writer::endStringMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginCompoundMessage() {
}

void Writer::visitCompoundMessage_x(uint32_t x) {
  _bb.writeVarUint(1);
  _bb.writeVarUint(x);
}

void Writer::visitCompoundMessage_y(uint32_t y) {
  _bb.writeVarUint(2);
  _bb.writeVarUint(y);
}

void Writer::endCompoundMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginNestedMessage() {
}

void Writer::visitNestedMessage_a(uint32_t a) {
  _bb.writeVarUint(1);
  _bb.writeVarUint(a);
}

void Writer::visitNestedMessage_b() {
  _bb.writeVarUint(2);
}

void Writer::visitNestedMessage_c(uint32_t c) {
  _bb.writeVarUint(3);
  _bb.writeVarUint(c);
}

void Writer::endNestedMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginBoolArrayStruct() {
}

void Writer::visitBoolArrayStruct_x_count(uint32_t size) {
  _bb.writeVarUint(size);
}

void Writer::visitBoolArrayStruct_x_element(bool x) {
  _bb.writeByte(x);
}

void Writer::endBoolArrayStruct() {
}

void Writer::beginByteArrayStruct() {
}

void Writer::visitByteArrayStruct_x_count(uint32_t size) {
  _bb.writeVarUint(size);
}

void Writer::visitByteArrayStruct_x_element(uint8_t x) {
  _bb.writeByte(x);
}

void Writer::endByteArrayStruct() {
}

void Writer::beginIntArrayStruct() {
}

void Writer::visitIntArrayStruct_x_count(uint32_t size) {
  _bb.writeVarUint(size);
}

void Writer::visitIntArrayStruct_x_element(int32_t x) {
  _bb.writeVarInt(x);
}

void Writer::endIntArrayStruct() {
}

void Writer::beginUintArrayStruct() {
}

void Writer::visitUintArrayStruct_x_count(uint32_t size) {
  _bb.writeVarUint(size);
}

void Writer::visitUintArrayStruct_x_element(uint32_t x) {
  _bb.writeVarUint(x);
}

void Writer::endUintArrayStruct() {
}

void Writer::beginInt64ArrayStruct() {
}

void Writer::visitInt64ArrayStruct_x_count(uint32_t size) {
  _bb.writeVarUint(size);
}

void Writer::visitInt64ArrayStruct_x_element(int64_t x) {
  _bb.writeVarInt64(x);
}

void Writer::endInt64ArrayStruct() {
}

void Writer::beginUint64ArrayStruct() {
}

void Writer::visitUint64ArrayStruct_x_count(uint32_t size) {
  _bb.writeVarUint(size);
}

void Writer::visitUint64ArrayStruct_x_element(uint64_t x) {
  _bb.writeVarUint64(x);
}

void Writer::endUint64ArrayStruct() {
}

void Writer::beginFloatArrayStruct() {
}

void Writer::visitFloatArrayStruct_x_count(uint32_t size) {
  _bb.writeVarUint(size);
}

void Writer::visitFloatArrayStruct_x_element(float x) {
  _bb.writeVarFloat(x);
}

void Writer::endFloatArrayStruct() {
}

void Writer::beginStringArrayStruct() {
}

void Writer::visitStringArrayStruct_x_count(uint32_t size) {
  _bb.writeVarUint(size);
}

void Writer::visitStringArrayStruct_x_element(const char *x) {
  _bb.writeString(x);
}

void Writer::endStringArrayStruct() {
}

void Writer::beginCompoundArrayStruct() {
}

void Writer::visitCompoundArrayStruct_x_count(uint32_t size) {
  _bb.writeVarUint(size);
}

void Writer::visitCompoundArrayStruct_x_element(uint32_t x) {
  _bb.writeVarUint(x);
}

void Writer::visitCompoundArrayStruct_y_count(uint32_t size) {
  _bb.writeVarUint(size);
}

void Writer::visitCompoundArrayStruct_y_element(uint32_t y) {
  _bb.writeVarUint(y);
}

void Writer::endCompoundArrayStruct() {
}

void Writer::beginBoolArrayMessage() {
}

void Writer::visitBoolArrayMessage_x_count(uint32_t size) {
  _bb.writeVarUint(1);
  _bb.writeVarUint(size);
}

void Writer::visitBoolArrayMessage_x_element(bool x) {
  _bb.writeByte(x);
}

void Writer::endBoolArrayMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginByteArrayMessage() {
}

void Writer::visitByteArrayMessage_x_count(uint32_t size) {
  _bb.writeVarUint(1);
  _bb.writeVarUint(size);
}

void Writer::visitByteArrayMessage_x_element(uint8_t x) {
  _bb.writeByte(x);
}

void Writer::endByteArrayMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginIntArrayMessage() {
}

void Writer::visitIntArrayMessage_x_count(uint32_t size) {
  _bb.writeVarUint(1);
  _bb.writeVarUint(size);
}

void Writer::visitIntArrayMessage_x_element(int32_t x) {
  _bb.writeVarInt(x);
}

void Writer::endIntArrayMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginUintArrayMessage() {
}

void Writer::visitUintArrayMessage_x_count(uint32_t size) {
  _bb.writeVarUint(1);
  _bb.writeVarUint(size);
}

void Writer::visitUintArrayMessage_x_element(uint32_t x) {
  _bb.writeVarUint(x);
}

void Writer::endUintArrayMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginInt64ArrayMessage() {
}

void Writer::visitInt64ArrayMessage_x_count(uint32_t size) {
  _bb.writeVarUint(1);
  _bb.writeVarUint(size);
}

void Writer::visitInt64ArrayMessage_x_element(int64_t x) {
  _bb.writeVarInt64(x);
}

void Writer::endInt64ArrayMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginUint64ArrayMessage() {
}

void Writer::visitUint64ArrayMessage_x_count(uint32_t size) {
  _bb.writeVarUint(1);
  _bb.writeVarUint(size);
}

void Writer::visitUint64ArrayMessage_x_element(uint64_t x) {
  _bb.writeVarUint64(x);
}

void Writer::endUint64ArrayMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginFloatArrayMessage() {
}

void Writer::visitFloatArrayMessage_x_count(uint32_t size) {
  _bb.writeVarUint(1);
  _bb.writeVarUint(size);
}

void Writer::visitFloatArrayMessage_x_element(float x) {
  _bb.writeVarFloat(x);
}

void Writer::endFloatArrayMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginStringArrayMessage() {
}

void Writer::visitStringArrayMessage_x_count(uint32_t size) {
  _bb.writeVarUint(1);
  _bb.writeVarUint(size);
}

void Writer::visitStringArrayMessage_x_element(const char *x) {
  _bb.writeString(x);
}

void Writer::endStringArrayMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginCompoundArrayMessage() {
}

void Writer::visitCompoundArrayMessage_x_count(uint32_t size) {
  _bb.writeVarUint(1);
  _bb.writeVarUint(size);
}

void Writer::visitCompoundArrayMessage_x_element(uint32_t x) {
  _bb.writeVarUint(x);
}

void Writer::visitCompoundArrayMessage_y_count(uint32_t size) {
  _bb.writeVarUint(2);
  _bb.writeVarUint(size);
}

void Writer::visitCompoundArrayMessage_y_element(uint32_t y) {
  _bb.writeVarUint(y);
}

void Writer::endCompoundArrayMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginRecursiveMessage() {
}

void Writer::visitRecursiveMessage_x() {
  _bb.writeVarUint(1);
}

void Writer::endRecursiveMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginNonDeprecatedMessage() {
}

void Writer::visitNonDeprecatedMessage_a(uint32_t a) {
  _bb.writeVarUint(1);
  _bb.writeVarUint(a);
}

void Writer::visitNonDeprecatedMessage_b(uint32_t b) {
  _bb.writeVarUint(2);
  _bb.writeVarUint(b);
}

void Writer::visitNonDeprecatedMessage_c_count(uint32_t size) {
  _bb.writeVarUint(3);
  _bb.writeVarUint(size);
}

void Writer::visitNonDeprecatedMessage_c_element(uint32_t c) {
  _bb.writeVarUint(c);
}

void Writer::visitNonDeprecatedMessage_d_count(uint32_t size) {
  _bb.writeVarUint(4);
  _bb.writeVarUint(size);
}

void Writer::visitNonDeprecatedMessage_d_element(uint32_t d) {
  _bb.writeVarUint(d);
}

void Writer::visitNonDeprecatedMessage_e(uint8_t e_x) {
  _bb.writeVarUint(5);
  _bb.writeByte(e_x);
}

void Writer::visitNonDeprecatedMessage_f(uint8_t f_x) {
  _bb.writeVarUint(6);
  _bb.writeByte(f_x);
}

void Writer::visitNonDeprecatedMessage_g(uint32_t g) {
  _bb.writeVarUint(7);
  _bb.writeVarUint(g);
}

void Writer::endNonDeprecatedMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginDeprecatedMessage() {
}

void Writer::visitDeprecatedMessage_a(uint32_t a) {
  _bb.writeVarUint(1);
  _bb.writeVarUint(a);
}

void Writer::visitDeprecatedMessage_c_count(uint32_t size) {
  _bb.writeVarUint(3);
  _bb.writeVarUint(size);
}

void Writer::visitDeprecatedMessage_c_element(uint32_t c) {
  _bb.writeVarUint(c);
}

void Writer::visitDeprecatedMessage_e(uint8_t e_x) {
  _bb.writeVarUint(5);
  _bb.writeByte(e_x);
}

void Writer::visitDeprecatedMessage_g(uint32_t g) {
  _bb.writeVarUint(7);
  _bb.writeVarUint(g);
}

void Writer::endDeprecatedMessage() {
  _bb.writeVarUint(0);
}

void Writer::beginSortedStruct() {
}

void Writer::visitSortedStruct_a1(bool a1) {
  _bb.writeByte(a1);
}

void Writer::visitSortedStruct_b1(uint8_t b1) {
  _bb.writeByte(b1);
}

void Writer::visitSortedStruct_c1(int32_t c1) {
  _bb.writeVarInt(c1);
}

void Writer::visitSortedStruct_d1(uint32_t d1) {
  _bb.writeVarUint(d1);
}

void Writer::visitSortedStruct_e1(float e1) {
  _bb.writeVarFloat(e1);
}

void Writer::visitSortedStruct_f1(const char *f1) {
  _bb.writeString(f1);
}

void Writer::visitSortedStruct_g1(int64_t g1) {
  _bb.writeVarInt64(g1);
}

void Writer::visitSortedStruct_h1(uint64_t h1) {
  _bb.writeVarUint64(h1);
}

void Writer::visitSortedStruct_a2(bool a2) {
  _bb.writeByte(a2);
}

void Writer::visitSortedStruct_b2(uint8_t b2) {
  _bb.writeByte(b2);
}

void Writer::visitSortedStruct_c2(int32_t c2) {
  _bb.writeVarInt(c2);
}

void Writer::visitSortedStruct_d2(uint32_t d2) {
  _bb.writeVarUint(d2);
}

void Writer::visitSortedStruct_e2(float e2) {
  _bb.writeVarFloat(e2);
}

void Writer::visitSortedStruct_f2(const char *f2) {
  _bb.writeString(f2);
}

void Writer::visitSortedStruct_g2(int64_t g2) {
  _bb.writeVarInt64(g2);
}

void Writer::visitSortedStruct_h2(uint64_t h2) {
  _bb.writeVarUint64(h2);
}

void Writer::visitSortedStruct_a3_count(uint32_t size) {
  _bb.writeVarUint(size);
}

void Writer::visitSortedStruct_a3_element(bool a3) {
  _bb.writeByte(a3);
}

void Writer::visitSortedStruct_b3_count(uint32_t size) {
  _bb.writeVarUint(size);
}

void Writer::visitSortedStruct_b3_element(uint8_t b3) {
  _bb.writeByte(b3);
}

void Writer::visitSortedStruct_c3_count(uint32_t size) {
  _bb.writeVarUint(size);
}

void Writer::visitSortedStruct_c3_element(int32_t c3) {
  _bb.writeVarInt(c3);
}

void Writer::visitSortedStruct_d3_count(uint32_t size) {
  _bb.writeVarUint(size);
}

void Writer::visitSortedStruct_d3_element(uint32_t d3) {
  _bb.writeVarUint(d3);
}

void Writer::visitSortedStruct_e3_count(uint32_t size) {
  _bb.writeVarUint(size);
}

void Writer::visitSortedStruct_e3_element(float e3) {
  _bb.writeVarFloat(e3);
}

void Writer::visitSortedStruct_f3_count(uint32_t size) {
  _bb.writeVarUint(size);
}

void Writer::visitSortedStruct_f3_element(const char *f3) {
  _bb.writeString(f3);
}

void Writer::visitSortedStruct_g3_count(uint32_t size) {
  _bb.writeVarUint(size);
}

void Writer::visitSortedStruct_g3_element(int64_t g3) {
  _bb.writeVarInt64(g3);
}

void Writer::visitSortedStruct_h3_count(uint32_t size) {
  _bb.writeVarUint(size);
}

void Writer::visitSortedStruct_h3_element(uint64_t h3) {
  _bb.writeVarUint64(h3);
}

void Writer::endSortedStruct() {
}

#endif

}

#endif
