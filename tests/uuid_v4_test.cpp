#include "uuid_v4.h"
#include "gtest/gtest.h"

namespace {
  void pb(const std::string &s) {
    for (int i=0; i<16; i++) {
      printf("%02hhx", s[i]);
    }
    printf("\n");
  }

  bool isBinaryLE(uint64_t x, uint64_t y, const std::string &bytes) {
    for (int i=0; i<8; i++) {
      if (static_cast<unsigned char>(bytes[i]) != ((x >> i*8) & 0xFF)) {
        return false;
      }
    }
    for (int i=8; i<16; i++) {
      if (static_cast<unsigned char>(bytes[i]) != ((y >> (i-8)*8) & 0xFF)) {
        return false;
      }
    }
    return true;
  }

  TEST(UUIDTest, SerializeUUIDinLE) {
    uint64_t x = 0x0012003400560078ull;
    uint64_t y = 0x0012003400560078ull;
    UUIDv4::UUID uuid(x, y);
    std::string bytes = uuid.bytes();
    ASSERT_PRED3(isBinaryLE, x, y, bytes);
  }

  TEST(UUIDTest, PrettyPrints) {
    uint8_t bytes[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    UUIDv4::UUID uuid(bytes);
    std::string pretty = uuid.str();
    ASSERT_EQ(pretty, "00010203-0405-0607-0809-0a0b0c0d0e0f");
  }

  TEST(UUIDTest, UnserializeFromLE) {
    std::string bytes = {
      0x78, 0x00, 0x56, 0x00, 0x34, 0x00, 0x12, 0x00,
      0x78, 0x00, 0x56, 0x00, 0x34, 0x00, 0x12, 0x00
    };
    UUIDv4::UUID uuid(bytes);
    ASSERT_EQ(uuid.str(), "78005600-3400-1200-7800-560034001200");
  }

  TEST(UUIDTest, ParsePretty) {
    std::string bytes = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
      0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };
    UUIDv4::UUID uuid = UUIDv4::UUID::fromStrFactory("00010203-0405-0607-0809-0a0b0c0d0e0f");
    ASSERT_EQ(uuid.bytes(), bytes);
  }

  TEST(UUIDTest, StreamOperators) {
    UUIDv4::UUID uuid;
    std::string pretty = "00120034-0056-0078-0012-003400560078";
    std::istringstream in(pretty);
    std::ostringstream out;

    in >> uuid;
    out << uuid;

    ASSERT_EQ(out.str(), pretty);
  }

  TEST(UUIDTest, Comparisons) {
    UUIDv4::UUID uuid = UUIDv4::UUID::fromStrFactory("00120034-0056-0078-0012-003400560078");
    UUIDv4::UUID uuid2 = UUIDv4::UUID(uuid);

    ASSERT_TRUE(uuid == uuid2);
    ASSERT_FALSE(uuid < uuid2);

    UUIDv4::UUID uuid3 = UUIDv4::UUID::fromStrFactory("f0120034-0056-0078-0012-003400560078");
    ASSERT_TRUE(uuid < uuid3);

    UUIDv4::UUID uuid4 = UUIDv4::UUID::fromStrFactory("00020034-0056-0078-0012-003400560078");
    ASSERT_FALSE(uuid < uuid4);
    ASSERT_TRUE(uuid > uuid4);

    UUIDv4::UUID uuid5 = UUIDv4::UUID::fromStrFactory("fc120034-0056-0078-0012-003400560078");
    ASSERT_TRUE(uuid < uuid5);
    ASSERT_FALSE(uuid > uuid5);
  }

  TEST(UUIDTest, HashTest) {
    UUIDv4::UUID uuid = UUIDv4::UUID::fromStrFactory("00120034-0056-0078-0012-003400560078");
    UUIDv4::UUID uuid2 = UUIDv4::UUID(uuid);

    ASSERT_TRUE(uuid.hash() == uuid2.hash());
  }
}
