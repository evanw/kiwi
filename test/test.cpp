// ../cli.js --schema test-schema.kiwi --cpp test-schema.h && c++ test.cpp -std=c++11 -I.. && ./a.out

#include "test-schema.h"
#include <vector>

static void testStructBool() {
  puts("testStructBool");

  auto check = [](bool i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::BoolStructEncoder(bb).add_x(i);
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    bool value = false;
    bool success = test_cpp::BoolStructDecoder(bb2).read_x(value);
    assert(success);
    assert(value == i);
  };

  check(false, {0});
  check(true, {1});
}

static void testStructByte() {
  puts("testStructByte");

  auto check = [](uint8_t i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::ByteStructEncoder(bb).add_x(i);
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    uint8_t value = 0;
    bool success = test_cpp::ByteStructDecoder(bb2).read_x(value);
    assert(success);
    assert(value == i);
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
    test_cpp::UintStructEncoder(bb).add_x(i);
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    uint32_t value = 0;
    bool success = test_cpp::UintStructDecoder(bb2).read_x(value);
    assert(success);
    assert(value == i);
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
    test_cpp::IntStructEncoder(bb).add_x(i);
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    int32_t value = 0;
    bool success = test_cpp::IntStructDecoder(bb2).read_x(value);
    assert(success);
    assert(value == i);
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
    test_cpp::FloatStructEncoder(bb).add_x(i);
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    float value = 0;
    bool success = test_cpp::FloatStructDecoder(bb2).read_x(value);
    assert(success);
    assert(value == i);
  };

  check(0, {0, 0, 0, 0});
  check(1, {0, 0, 128, 63});
  check(-1, {0, 0, 128, 191});
  check(3.1415927410125732, {219, 15, 73, 64});
  check(-3.1415927410125732, {219, 15, 73, 192});
}

static void testStructString() {
  puts("testStructString");

  auto check = [](std::string i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::StringStructEncoder(bb).add_x(i);
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    std::string value;
    bool success = test_cpp::StringStructDecoder(bb2).read_x(value);
    assert(success);
    assert(value == i);
  };

  check("", {0});
  check("abc", {3, 97, 98, 99});
  check({"\0abc\0", 5}, {5, 0, 97, 98, 99, 0});
  check("🙉🙈🙊", {12, 240, 159, 153, 137, 240, 159, 153, 136, 240, 159, 153, 138});
}

static void testStructCompound() {
  puts("testStructCompound");

  auto check = [](uint32_t x, uint32_t y, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::CompoundStructEncoder(bb).add_x(x).add_y(y);
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test_cpp::CompoundStructDecoder decoder(bb2);
    uint32_t value_x = 0;
    uint32_t value_y = 0;
    bool success_x = decoder.read_x(value_x);
    bool success_y = decoder.read_y(value_y);
    assert(success_x);
    assert(success_y);
    assert(value_x == x);
    assert(value_y == y);
  };

  check(0, 0, {0, 0});
  check(1, 2, {1, 2});
  check(12345, 54321, {185, 96, 177, 168, 3});
}

static void testStructNested() {
  puts("testStructNested");

  auto check = [](uint32_t a, uint32_t bx, uint32_t by, uint32_t c, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::NestedStructEncoder encoder(bb);
    encoder.add_a(a);
    encoder.add_b().add_x(bx).add_y(by);
    encoder.add_c(c);
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test_cpp::NestedStructDecoder decoder(bb2);
    uint32_t value_a = 0;
    uint32_t value_bx = 0;
    uint32_t value_by = 0;
    uint32_t value_c = 0;
    bool success_a = decoder.read_a(value_a);
    test_cpp::CompoundStructDecoder decoder2;
    bool success_b = decoder.read_b(decoder2);
    bool success_bx = decoder2.read_x(value_bx);
    bool success_by = decoder2.read_y(value_by);
    bool success_c = decoder.read_c(value_c);

    assert(success_a);
    assert(success_b);
    assert(success_bx);
    assert(success_by);
    assert(success_c);
    assert(value_a == a);
    assert(value_bx == bx);
    assert(value_by == by);
    assert(value_c == c);
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
    test_cpp::BoolMessageEncoder(bb).finish();
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test_cpp::BoolMessageDecoder decoder(bb2);

    test_cpp::BoolMessageDecoder::Field next;
    bool success = decoder.nextField(next);
    assert(!success);
  }

  auto check = [](bool i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::BoolMessageEncoder(bb).add_x(i).finish();
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test_cpp::BoolMessageDecoder decoder(bb2);

    test_cpp::BoolMessageDecoder::Field next;
    bool success = decoder.nextField(next);
    assert(success);
    assert(next == test_cpp::BoolMessageDecoder::X);

    bool value = false;
    bool success2 = decoder.read_x(value);
    assert(success2);
    assert(value == i);

    bool success3 = decoder.nextField(next);
    assert(!success3);
  };

  check(false, {1, 0, 0});
  check(true, {1, 1, 0});
}

static void testMessageByte() {
  puts("testMessageByte");

  auto check = [](uint8_t i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::ByteMessageEncoder(bb).add_x(i).finish();
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test_cpp::ByteMessageDecoder decoder(bb2);

    test_cpp::ByteMessageDecoder::Field next;
    bool success = decoder.nextField(next);
    assert(success);
    assert(next == test_cpp::ByteMessageDecoder::X);

    uint8_t value = 0;
    bool success2 = decoder.read_x(value);
    assert(success2);
    assert(value == i);

    bool success3 = decoder.nextField(next);
    assert(!success3);
  };

  check(234, {1, 234, 0});
}

static void testMessageUint() {
  puts("testMessageUint");

  auto check = [](uint32_t i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::UintMessageEncoder(bb).add_x(i).finish();
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test_cpp::UintMessageDecoder decoder(bb2);

    test_cpp::UintMessageDecoder::Field next;
    bool success = decoder.nextField(next);
    assert(success);
    assert(next == test_cpp::UintMessageDecoder::X);

    uint32_t value = 0;
    bool success2 = decoder.read_x(value);
    assert(success2);
    assert(value == i);

    bool success3 = decoder.nextField(next);
    assert(!success3);
  };

  check(12345678, {1, 206, 194, 241, 5, 0});
}

static void testMessageInt() {
  puts("testMessageInt");

  auto check = [](int32_t i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::IntMessageEncoder(bb).add_x(i).finish();
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test_cpp::IntMessageDecoder decoder(bb2);

    test_cpp::IntMessageDecoder::Field next;
    bool success = decoder.nextField(next);
    assert(success);
    assert(next == test_cpp::IntMessageDecoder::X);

    int32_t value = 0;
    bool success2 = decoder.read_x(value);
    assert(success2);
    assert(value == i);

    bool success3 = decoder.nextField(next);
    assert(!success3);
  };

  check(12345678, {1, 156, 133, 227, 11, 0});
}

static void testMessageFloat() {
  puts("testMessageFloat");

  auto check = [](float i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::FloatMessageEncoder(bb).add_x(i).finish();
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test_cpp::FloatMessageDecoder decoder(bb2);

    test_cpp::FloatMessageDecoder::Field next;
    bool success = decoder.nextField(next);
    assert(success);
    assert(next == test_cpp::FloatMessageDecoder::X);

    float value = 0;
    bool success2 = decoder.read_x(value);
    assert(success2);
    assert(value == i);

    bool success3 = decoder.nextField(next);
    assert(!success3);
  };

  check(3.1415927410125732, {1, 219, 15, 73, 64, 0});
}

static void testMessageString() {
  puts("testMessageString");

  auto check = [](std::string i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::StringMessageEncoder(bb).add_x(i).finish();
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test_cpp::StringMessageDecoder decoder(bb2);

    test_cpp::StringMessageDecoder::Field next;
    bool success = decoder.nextField(next);
    assert(success);
    assert(next == test_cpp::StringMessageDecoder::X);

    std::string value;
    bool success2 = decoder.read_x(value);
    assert(success2);
    assert(value == i);

    bool success3 = decoder.nextField(next);
    assert(!success3);
  };

  check("", {1, 0, 0});
  check("🙉🙈🙊", {1, 12, 240, 159, 153, 137, 240, 159, 153, 136, 240, 159, 153, 138, 0});
}

static void testMessageCompound() {
  puts("testMessageCompound");

  auto check_xy = [](uint32_t x, uint32_t y, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::CompoundMessageEncoder(bb).add_x(x).add_y(y).finish();
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test_cpp::CompoundMessageDecoder decoder(bb2);

    test_cpp::CompoundMessageDecoder::Field next;
    bool success = decoder.nextField(next);
    assert(success);
    assert(next == test_cpp::CompoundMessageDecoder::X);

    uint32_t value_x = 0;
    bool success2 = decoder.read_x(value_x);
    assert(success2);
    assert(value_x == x);

    bool success3 = decoder.nextField(next);
    assert(success3);
    assert(next == test_cpp::CompoundMessageDecoder::Y);

    uint32_t value_y = 0;
    bool success4 = decoder.read_y(value_y);
    assert(success4);
    assert(value_y == y);

    bool success5 = decoder.nextField(next);
    assert(!success5);
  };

  auto check_yx = [](uint32_t y, uint32_t x, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::CompoundMessageEncoder(bb).add_y(y).add_x(x).finish();
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test_cpp::CompoundMessageDecoder decoder(bb2);

    test_cpp::CompoundMessageDecoder::Field next;
    bool success = decoder.nextField(next);
    assert(success);
    assert(next == test_cpp::CompoundMessageDecoder::Y);

    uint32_t value_y = 0;
    bool success2 = decoder.read_y(value_y);
    assert(success2);
    assert(value_y == y);

    bool success3 = decoder.nextField(next);
    assert(success3);
    assert(next == test_cpp::CompoundMessageDecoder::X);

    uint32_t value_x = 0;
    bool success4 = decoder.read_x(value_x);
    assert(success4);
    assert(value_x == x);

    bool success5 = decoder.nextField(next);
    assert(!success5);
  };

  check_xy(123, 234, {1, 123, 2, 234, 1, 0});
  check_yx(234, 123, {2, 234, 1, 1, 123, 0});
}

static void testMessageNested() {
  puts("testMessageNested");

  auto check = [](uint32_t a, uint32_t bx, uint32_t by, uint32_t c, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::NestedMessageEncoder encoder(bb);
    encoder.add_a(a);
    encoder.add_b().add_x(bx).add_y(by).finish();
    encoder.add_c(c).finish();
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test_cpp::NestedMessageDecoder decoder(bb2);

    test_cpp::NestedMessageDecoder::Field next;
    bool success = decoder.nextField(next);
    assert(success);
    assert(next == test_cpp::NestedMessageDecoder::A);

    uint32_t value_a = 0;
    bool success2 = decoder.read_a(value_a);
    assert(success2);
    assert(value_a == a);

    bool success3 = decoder.nextField(next);
    assert(success3);
    assert(next == test_cpp::NestedMessageDecoder::B);

    test_cpp::CompoundMessageDecoder decoder2;
    bool success4 = decoder.read_b(decoder2);
    assert(success4);

    test_cpp::CompoundMessageDecoder::Field next2;
    bool success5 = decoder2.nextField(next2);
    assert(success5);
    assert(next2 == test_cpp::CompoundMessageDecoder::X);

    uint32_t value_x = 0;
    bool success6 = decoder2.read_x(value_x);
    assert(success6);
    assert(value_x == bx);

    bool success7 = decoder2.nextField(next2);
    assert(success7);
    assert(next2 == test_cpp::CompoundMessageDecoder::Y);

    uint32_t value_y = 0;
    bool success8 = decoder2.read_y(value_y);
    assert(success8);
    assert(value_y == by);

    bool success9 = decoder2.nextField(next2);
    assert(!success9);

    bool success10 = decoder.nextField(next);
    assert(success10);
    assert(next == test_cpp::NestedMessageDecoder::C);

    uint32_t value_c;
    bool success11 = decoder.read_c(value_c);
    assert(success11);
    assert(value_c == c);

    bool success12 = decoder.nextField(next);
    assert(!success12);
  };

  check(234, 5, 6, 123, {1, 234, 1, 2, 1, 5, 2, 6, 0, 3, 123, 0});
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

  puts("all tests passed");
  return 0;
}
