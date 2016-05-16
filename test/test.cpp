// ../cli.js --schema test-schema.kiwi --cpp test-schema.h && c++ test.cpp -std=c++11 -I.. && ./a.out

#define IMPLEMENT_KIWI_H
#define IMPLEMENT_SCHEMA_H

#include "test-schema.h"
#include <vector>

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

static void testRequiredField() {
  puts("testRequiredField");

  {
    kiwi::ByteBuffer bb;
    test::RequiredField s;
    assert(!s.encode(bb));
  }

  {
    kiwi::ByteBuffer bb;
    kiwi::MemoryPool pool;
    test::RequiredField s;
    s.set_x(0);
    assert(s.encode(bb));
  }

  {
    std::vector<uint8_t> o{0};
    kiwi::ByteBuffer bb(o.data(), o.size());
    kiwi::MemoryPool pool;
    test::RequiredField s;
    assert(!s.decode(bb, pool));
  }

  {
    std::vector<uint8_t> o{1, 0, 0};
    kiwi::ByteBuffer bb(o.data(), o.size());
    kiwi::MemoryPool pool;
    test::RequiredField s;
    assert(s.decode(bb, pool));
  }
}

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
  testRequiredField();

  puts("all tests passed");
  return 0;
}
