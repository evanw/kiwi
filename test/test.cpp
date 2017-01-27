// ../cli.js --schema test-schema.kiwi --cpp test-schema.h && c++ test.cpp -std=c++11 -I.. && ./a.out

#define IMPLEMENT_KIWI_H
#define IMPLEMENT_SCHEMA_H

#include "test-schema.h"
#include "test1-schema.h"
#include "test2-schema.h"
#include "test-schema-large.h"
#include <stdio.h>
#include <vector>

static bool readFile(const char *path, kiwi::ByteBuffer &bb) {
  if (FILE *f = fopen(path, "rb")) {
    int c;
    while ((c = fgetc(f)) != EOF) {
      bb.writeByte(c);
    }
    fclose(f);
    return true;
  }
  return false;
}

static void testStructBool() {
  puts("testStructBool");

  auto check = [](bool i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    kiwi::MemoryPool pool;

    test::BoolStruct s;
    s.set_x(i);
    assert(s.encode(bb));
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test::BoolStruct s2;
    assert(s2.decode(bb2, pool));
    assert(s2.x());
    assert(*s2.x() == i);
  };

  check(false, {0});
  check(true, {1});
}

static void testStructByte() {
  puts("testStructByte");

  auto check = [](uint8_t i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    kiwi::MemoryPool pool;

    test::ByteStruct s;
    s.set_x(i);
    assert(s.encode(bb));
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test::ByteStruct s2;
    assert(s2.decode(bb2, pool));
    assert(s2.x());
    assert(*s2.x() == i);
  };

  check(0x00, {0x00});
  check(0x01, {0x01});
  check(0x7F, {0x7F});
  check(0x80, {0x80});
  check(0xFF, {0xFF});
}

static void testStructUint() {
  puts("testStructUint");

  auto check = [](uint32_t i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    kiwi::MemoryPool pool;

    test::UintStruct s;
    s.set_x(i);
    assert(s.encode(bb));
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test::UintStruct s2;
    assert(s2.decode(bb2, pool));
    assert(s2.x());
    assert(*s2.x() == i);
  };

  check(0x00, {0x00});
  check(0x01, {0x01});
  check(0x02, {0x02});
  check(0x7F, {0x7F});
  check(0x80, {0x80, 0x01});
  check(0x81, {0x81, 0x01});
  check(0x100, {0x80, 0x02});
  check(0x101, {0x81, 0x02});
  check(0x17F, {0xFF, 0x02});
  check(0x180, {0x80, 0x03});
  check(0x1FF, {0xFF, 0x03});
  check(0x200, {0x80, 0x04});
  check(0x7FFF, {0xFF, 0xFF, 0x01});
  check(0x8000, {0x80, 0x80, 0x02});
  check(0x7FFFFFFF, {0xFF, 0xFF, 0xFF, 0xFF, 0x07});
  check(0x80000000, {0x80, 0x80, 0x80, 0x80, 0x08});
}

static void testStructInt() {
  puts("testStructInt");

  auto check = [](int32_t i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    kiwi::MemoryPool pool;

    test::IntStruct s;
    s.set_x(i);
    assert(s.encode(bb));
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test::IntStruct s2;
    assert(s2.decode(bb2, pool));
    assert(s2.x());
    assert(*s2.x() == i);
  };

  check(0x00, {0x00});
  check(-0x01, {0x01});
  check(0x01, {0x02});
  check(-0x02, {0x03});
  check(0x02, {0x04});
  check(-0x3F, {0x7D});
  check(0x3F, {0x7E});
  check(-0x40, {0x7F});
  check(0x40, {0x80, 0x01});
  check(-0x3FFF, {0xFD, 0xFF, 0x01});
  check(0x3FFF, {0xFE, 0xFF, 0x01});
  check(-0x4000, {0xFF, 0xFF, 0x01});
  check(0x4000, {0x80, 0x80, 0x02});
  check(-0x3FFFFFFF, {0xFD, 0xFF, 0xFF, 0xFF, 0x07});
  check(0x3FFFFFFF, {0xFE, 0xFF, 0xFF, 0xFF, 0x07});
  check(-0x40000000, {0xFF, 0xFF, 0xFF, 0xFF, 0x07});
  check(0x40000000, {0x80, 0x80, 0x80, 0x80, 0x08});
  check(-0x7FFFFFFF, {0xFD, 0xFF, 0xFF, 0xFF, 0x0F});
  check(0x7FFFFFFF, {0xFE, 0xFF, 0xFF, 0xFF, 0x0F});
  check(-0x80000000, {0xFF, 0xFF, 0xFF, 0xFF, 0x0F});
}

static void testStructFloat() {
  puts("testStructFloat");

  auto check = [](float i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    kiwi::MemoryPool pool;

    test::FloatStruct s;
    s.set_x(i);
    assert(s.encode(bb));
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test::FloatStruct s2;
    assert(s2.decode(bb2, pool));
    assert(s2.x());
    assert(*s2.x() == i || (*s2.x() != *s2.x() && i != i));
  };

  check(0, {0});
  check(1, {127, 0, 0, 0});
  check(-1, {127, 1, 0, 0});
  check(3.1415927410125732, {128, 182, 31, 146});
  check(-3.1415927410125732, {128, 183, 31, 146});
  check(1.0 / 0, {255, 0, 0, 0});
  check(-1.0 / 0, {255, 1, 0, 0});
  check(0.0 / 0, {255, 0, 0, 128});
}

static void testStructString() {
  puts("testStructString");

  auto check = [](const char *i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    kiwi::MemoryPool pool;

    test::StringStruct s;
    s.set_x(pool.string(i));
    assert(s.encode(bb));
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test::StringStruct s2;
    assert(s2.decode(bb2, pool));
    assert(s2.x());
    assert(*s2.x() == kiwi::String(i));
  };

  check("", {0});
  check("\0abc\0", {0});
  check("abc", {97, 98, 99, 0});
  check("🙉🙈🙊", {240, 159, 153, 137, 240, 159, 153, 136, 240, 159, 153, 138, 0});
}

static void testStructCompound() {
  puts("testStructCompound");

  auto check = [](uint32_t x, uint32_t y, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    kiwi::MemoryPool pool;

    test::CompoundStruct s;
    s.set_x(x);
    s.set_y(y);
    assert(s.encode(bb));
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test::CompoundStruct s2;
    assert(s2.decode(bb2, pool));
    assert(s2.x());
    assert(s2.y());
    assert(*s2.x() == x);
    assert(*s2.y() == y);
  };

  check(0, 0, {0, 0});
  check(1, 2, {1, 2});
  check(12345, 54321, {185, 96, 177, 168, 3});
}

static void testStructNested() {
  puts("testStructNested");

  auto check = [](uint32_t a, uint32_t bx, uint32_t by, uint32_t c, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    kiwi::MemoryPool pool;

    test::NestedStruct s;
    s.set_a(a);
    test::CompoundStruct b;
    s.set_b(&b);
    b.set_x(bx);
    b.set_y(by);
    s.set_c(c);
    assert(s.encode(bb));
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test::NestedStruct s2;
    assert(s2.decode(bb2, pool));
    assert(s2.a());
    assert(s2.b());
    assert(s2.b()->x());
    assert(s2.b()->y());
    assert(s2.c());
    assert(*s2.a() == a);
    assert(*s2.b()->x() == bx);
    assert(*s2.b()->y() == by);
    assert(*s2.c() == c);
  };

  check(0, 0, 0, 0, {0, 0, 0, 0});
  check(1, 2, 3, 4, {1, 2, 3, 4});
  check(534, 12345, 54321, 321, {150, 4, 185, 96, 177, 168, 3, 193, 2});
}

static void testMessageBool() {
  puts("testMessageBool");

  {
    std::vector<uint8_t> o{0};
    kiwi::ByteBuffer bb;
    kiwi::MemoryPool pool;

    test::BoolMessage s;
    assert(s.encode(bb));
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test::BoolMessage s2;
    assert(s2.decode(bb2, pool));
    assert(!s2.x());
  }

  auto check = [](bool i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    kiwi::MemoryPool pool;

    test::BoolMessage s;
    s.set_x(i);
    assert(s.encode(bb));
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test::BoolMessage s2;
    assert(s2.decode(bb2, pool));
    assert(s2.x());
    assert(*s2.x() == i);
  };

  check(false, {1, 0, 0});
  check(true, {1, 1, 0});
}

static void testMessageByte() {
  puts("testMessageByte");

  auto check = [](uint8_t i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    kiwi::MemoryPool pool;

    test::ByteMessage s;
    s.set_x(i);
    assert(s.encode(bb));
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test::ByteMessage s2;
    assert(s2.decode(bb2, pool));
    assert(s2.x());
    assert(*s2.x() == i);
  };

  check(234, {1, 234, 0});
}

static void testMessageUint() {
  puts("testMessageUint");

  auto check = [](uint32_t i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    kiwi::MemoryPool pool;

    test::UintMessage s;
    s.set_x(i);
    assert(s.encode(bb));
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test::UintMessage s2;
    assert(s2.decode(bb2, pool));
    assert(s2.x());
    assert(*s2.x() == i);
  };

  check(12345678, {1, 206, 194, 241, 5, 0});
}

static void testMessageInt() {
  puts("testMessageInt");

  auto check = [](int32_t i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    kiwi::MemoryPool pool;

    test::IntMessage s;
    s.set_x(i);
    assert(s.encode(bb));
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test::IntMessage s2;
    assert(s2.decode(bb2, pool));
    assert(s2.x());
    assert(*s2.x() == i);
  };

  check(12345678, {1, 156, 133, 227, 11, 0});
}

static void testMessageFloat() {
  puts("testMessageFloat");

  auto check = [](float i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    kiwi::MemoryPool pool;

    test::FloatMessage s;
    s.set_x(i);
    assert(s.encode(bb));
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test::FloatMessage s2;
    assert(s2.decode(bb2, pool));
    assert(s2.x());
    assert(*s2.x() == i);
  };

  check(3.1415927410125732, {1, 128, 182, 31, 146, 0});
}

static void testMessageString() {
  puts("testMessageString");

  auto check = [](const char *i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    kiwi::MemoryPool pool;

    test::StringMessage s;
    s.set_x(pool.string(i));
    assert(s.encode(bb));
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test::StringMessage s2;
    assert(s2.decode(bb2, pool));
    assert(s2.x());
    assert(*s2.x() == kiwi::String(i));
  };

  check("", {1, 0, 0});
  check("\0abc\0", {1, 0, 0});
  check("abc", {1, 97, 98, 99, 0, 0});
  check("🙉🙈🙊", {1, 240, 159, 153, 137, 240, 159, 153, 136, 240, 159, 153, 138, 0, 0});
}

static void testMessageCompound() {
  puts("testMessageCompound");

  auto check = [](uint32_t x, uint32_t y, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    kiwi::MemoryPool pool;

    test::CompoundMessage s;
    if (x) s.set_x(x);
    if (y) s.set_y(y);
    assert(s.encode(bb));
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test::CompoundMessage s2;
    assert(s2.decode(bb2, pool));
    assert(!!s2.x() == !!x);
    assert(!!s2.y() == !!y);
    if (x) assert(*s2.x() == x);
    if (y) assert(*s2.y() == y);
  };

  check(0, 0, {0});
  check(123, 0, {1, 123, 0});
  check(0, 234, {2, 234, 1, 0});
  check(123, 234, {1, 123, 2, 234, 1, 0});
}

static void testMessageNested() {
  puts("testMessageNested");

  auto check = [](uint32_t a, uint32_t bx, uint32_t by, uint32_t c, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    kiwi::MemoryPool pool;

    test::NestedMessage s;
    if (a) s.set_a(a);
    if (bx || by) {
      auto b = pool.allocate<test::CompoundMessage>();
      s.set_b(b);
      if (bx) b->set_x(bx);
      if (by) b->set_y(by);
    }
    if (c) s.set_c(c);
    assert(s.encode(bb));
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test::NestedMessage s2;
    assert(s2.decode(bb2, pool));
    assert(!!s2.a() == !!a);
    assert(!!s2.b() == (bx || by));
    assert(!!s2.c() == !!c);
    if (a) assert(*s2.a() == a);
    if (bx) assert(*s2.b()->x() == bx);
    if (by) assert(*s2.b()->y() == by);
    if (c) assert(*s2.c() == c);
  };

  check(0, 0, 0, 0, {0});
  check(123, 0, 0, 234, {1, 123, 3, 234, 1, 0});
  check(0, 5, 6, 0, {2, 1, 5, 2, 6, 0, 0});
  check(0, 5, 0, 123, {2, 1, 5, 0, 3, 123, 0});
  check(234, 5, 6, 123, {1, 234, 1, 2, 1, 5, 2, 6, 0, 3, 123, 0});
}

static void testRecursiveMessage() {
  puts("testRecursiveMessage");

  std::vector<uint8_t> o{1, 1, 0, 0, 0};
  kiwi::ByteBuffer bb;
  kiwi::MemoryPool pool;

  test::RecursiveMessage a;
  test::RecursiveMessage b;
  test::RecursiveMessage c;

  a.set_x(&b);
  b.set_x(&c);
  assert(a.encode(bb));
  assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

  test::RecursiveMessage a2;
  assert(a2.decode(bb, pool));
  assert(a2.x());
  assert(a2.x()->x());
  assert(!a2.x()->x()->x());
}

static void testBinarySchema() {
  puts("testBinarySchema");

  test1::BinarySchema test1_schema1;
  test1::BinarySchema test1_schema2;
  test2::BinarySchema test2_schema1;
  test2::BinarySchema test2_schema2;

  {
    kiwi::ByteBuffer file;
    assert(readFile("test1-schema.bkiwi", file));
    assert(test1_schema1.parse(file));
  }

  {
    kiwi::ByteBuffer file;
    assert(readFile("test2-schema.bkiwi", file));
    assert(test1_schema2.parse(file));
  }

  {
    kiwi::ByteBuffer file;
    assert(readFile("test1-schema.bkiwi", file));
    assert(test2_schema1.parse(file));
  }

  {
    kiwi::ByteBuffer file;
    assert(readFile("test2-schema.bkiwi", file));
    assert(test2_schema2.parse(file));
  }

  auto check1 = [](const std::vector<uint8_t> &i, test1::BinarySchema *schema, const std::vector<uint8_t> &o) {
    kiwi::MemoryPool pool;
    kiwi::ByteBuffer bb(i.data(), i.size());
    test1::Message message;
    if (o.empty()) {
      assert(!message.decode(bb, pool, schema));
    } else {
      assert(message.decode(bb, pool, schema));
      kiwi::ByteBuffer bb2;
      assert(message.encode(bb2));
      assert(std::vector<uint8_t>(bb2.data(), bb2.data() + bb2.size()) == o);
    }
  };

  auto check2 = [](const std::vector<uint8_t> &i, test2::BinarySchema *schema, const std::vector<uint8_t> &o) {
    kiwi::MemoryPool pool;
    kiwi::ByteBuffer bb(i.data(), i.size());
    test2::Message message;
    if (o.empty()) {
      assert(!message.decode(bb, pool, schema));
    } else {
      assert(message.decode(bb, pool, schema));
      kiwi::ByteBuffer bb2;
      assert(message.encode(bb2));
      assert(std::vector<uint8_t>(bb2.data(), bb2.data() + bb2.size()) == o);
    }
  };

  std::vector<uint8_t> bytes1{1, 2, 2, 128, 0, 0, 0, 128, 0, 0, 128, 0};
  check1(bytes1, nullptr,        bytes1);
  check1(bytes1, &test1_schema1, bytes1);
  check1(bytes1, &test1_schema2, bytes1);
  check2(bytes1, nullptr,        bytes1);
  check2(bytes1, &test2_schema1, bytes1);
  check2(bytes1, &test2_schema2, bytes1);

  std::vector<uint8_t> bytes2{1, 2, 2, 128, 0, 0, 0, 128, 0, 0, 128, 3, 1, 0, 4, 0, 5, 1, 240, 159, 141, 149, 0, 0, 0};
  check1(bytes2, nullptr,        {});
  check1(bytes2, &test1_schema1, {});
  check1(bytes2, &test1_schema2, bytes1);
  check2(bytes2, nullptr,        bytes2);
  check2(bytes2, &test2_schema1, bytes2);
  check2(bytes2, &test2_schema2, bytes2);
}

#define SET_10(n0, n1, n2, n3, n4, n5, n6, n7, n8, n9)  \
  x.set_f ## n0(n0); \
  x.set_f ## n1(n1); \
  x.set_f ## n2(n2); \
  x.set_f ## n3(n3); \
  x.set_f ## n4(n4); \
  x.set_f ## n5(n5); \
  x.set_f ## n6(n6); \
  x.set_f ## n7(n7); \
  x.set_f ## n8(n8); \
  x.set_f ## n9(n9);

#define GET_10(n0, n1, n2, n3, n4, n5, n6, n7, n8, n9)  \
  ptr = x.f ## n0(); assert(ptr && *ptr == n0); \
  ptr = x.f ## n1(); assert(ptr && *ptr == n1); \
  ptr = x.f ## n2(); assert(ptr && *ptr == n2); \
  ptr = x.f ## n3(); assert(ptr && *ptr == n3); \
  ptr = x.f ## n4(); assert(ptr && *ptr == n4); \
  ptr = x.f ## n5(); assert(ptr && *ptr == n5); \
  ptr = x.f ## n6(); assert(ptr && *ptr == n6); \
  ptr = x.f ## n7(); assert(ptr && *ptr == n7); \
  ptr = x.f ## n8(); assert(ptr && *ptr == n8); \
  ptr = x.f ## n9(); assert(ptr && *ptr == n9);

#define REPEAT(OP) \
  OP ## _10(0, 1, 2, 3, 4, 5, 6, 7, 8, 9); \
  OP ## _10(10, 11, 12, 13, 14, 15, 16, 17, 18, 19); \
  OP ## _10(20, 21, 22, 23, 24, 25, 26, 27, 28, 29); \
  OP ## _10(30, 31, 32, 33, 34, 35, 36, 37, 38, 39); \
  OP ## _10(40, 41, 42, 43, 44, 45, 46, 47, 48, 49); \
  OP ## _10(50, 51, 52, 53, 54, 55, 56, 57, 58, 59); \
  OP ## _10(60, 61, 62, 63, 64, 65, 66, 67, 68, 69); \
  OP ## _10(70, 71, 72, 73, 74, 75, 76, 77, 78, 79); \
  OP ## _10(80, 81, 82, 83, 84, 85, 86, 87, 88, 89); \
  OP ## _10(90, 91, 92, 93, 94, 95, 96, 97, 98, 99); \
  OP ## _10(100, 101, 102, 103, 104, 105, 106, 107, 108, 109); \
  OP ## _10(110, 111, 112, 113, 114, 115, 116, 117, 118, 119); \
  OP ## _10(120, 121, 122, 123, 124, 125, 126, 127, 128, 129); \

static void testLargeStruct() {
  puts("testLargeStruct");

  test_large::Struct x;

  REPEAT(SET)

  kiwi::ByteBuffer bb;
  kiwi::MemoryPool pool;
  test_large::Struct x2;
  int *ptr;

  assert(x.encode(bb));
  assert(x2.decode(bb, pool));

  REPEAT(GET)
}

static void testLargeMessage() {
  puts("testLargeMessage");

  test_large::Message x;

  REPEAT(SET)

  kiwi::ByteBuffer bb;
  kiwi::MemoryPool pool;
  test_large::Message x2;
  int *ptr;

  assert(x.encode(bb));
  assert(x2.decode(bb, pool));

  REPEAT(GET)
}

#undef SET
#undef GET
#undef REPEAT

int main() {
  testStructBool();
  testStructByte();
  testStructUint();
  testStructInt();
  testStructFloat();
  testStructString();
  testStructCompound();
  testStructNested();

  testMessageBool();
  testMessageByte();
  testMessageUint();
  testMessageInt();
  testMessageFloat();
  testMessageString();
  testMessageCompound();
  testMessageNested();

  testRecursiveMessage();
  testBinarySchema();

  testLargeStruct();
  testLargeMessage();

  puts("all tests passed");
  return 0;
}
