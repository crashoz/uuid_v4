#pragma once

#include <random>
#include <string>
#include <limits>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <cstring>
#include <cstdint>
#include <cstdio>

//#define BIGENDIAN
#include "endianness.h"

/*
 * Converts the lowest 32-bits of x to a hex string (8 chars)
 */
void u32tos(uint64_t x, char *s)
{
  x = ((x & 0xFFFF) << 32) | ((x & 0xFFFF0000) >> 16);
  x = ((x & 0x0000FF000000FF00) >> 8) | (x & 0x000000FF000000FF) << 16;
  x = ((x & 0x00F000F000F000F0) >> 4) | (x & 0x000F000F000F000F) << 8;
  uint64_t mask = ((x + 0x0606060606060606) >> 4) & 0x0101010101010101;
  x |= 0x3030303030303030;
  x += 0x27 * mask;
  *reinterpret_cast<uint64_t*>(s) = x;
}

/*
 * Converts a hex string (8 chars) to an uint32_t
 */
uint32_t stou32(const char *s)
{
  uint64_t x = *reinterpret_cast<const uint64_t*>(s);
  x -= 0x2F2F2F2F2F2F2F2F;
  uint64_t mask = (x & 0x2020202020202020) >> 5;
  x -= 0x27 * mask;
  x -= 0x0101010101010101;
  x |= (x & 0x000F000F000F000F) << 12;
  x = ((x & 0xFF000000FF000000) >> 24) | (x & 0x0000FF000000FF00);
  x = ((x & 0x0000FFFF00000000) >> 32) | ((x & 0x000000000000FFFF) << 16);
  return x;
}

/*
 * UUIDv4 (random) RFC-4122
 */
class UUID {
  public:
    /* Builds a 128-bits UUID */
    UUID(const uint64_t x, const uint64_t y) : ab(x), cd(y)
    {}

    /* Builds an UUID from a byte string (16 bytes long) */
    UUID(const std::string &bytes) :
    ab(betole64(*reinterpret_cast<const uint64_t*>(bytes.c_str()))),
    cd(betole64(*reinterpret_cast<const uint64_t*>(bytes.c_str()+8)))
    {}

    static __attribute__((always_inline)) UUID fromStr(const std::string &s) {
      const char* raw = s.c_str();
      char buf[8];
      uint64_t x, y;
      x = static_cast<uint64_t>(stou32(raw)) << 32;
      memcpy(buf, raw+9, 4);
      memcpy(buf+4, raw+14, 4);
      x |= stou32(buf);
      memcpy(buf, raw+19, 4);
      memcpy(buf+4, raw+24, 4);
      y = (static_cast<uint64_t>(stou32(buf)) << 32) | (stou32(raw+28));
      return UUID(x, y);
    }

    bool operator==(const UUID &other) const {
      return ab == other.ab && cd == other.cd;
    }

    bool operator!=(const UUID &other) const {
      return !operator==(other);
    }

    bool operator<(const UUID &other) const {
      if(ab < other.ab)  {
        return true;
      }
      if(ab > other.ab) {
        return false;
      }
      if(cd < other.cd) {
        return true;
      }
      return false;
    }

    /* Serializes the UUID to a byte string (16 bytes long) */
    std::string bytes() const {
      char bytes[16];
      const uint64_t x = betole64(ab);
      const uint64_t y = betole64(cd);
      memcpy(bytes, &x, sizeof(uint64_t));
      memcpy(bytes+8, &y, sizeof(uint64_t));
      return std::string(bytes, 16);
    }


    std::string str() const {
      char hex[8];
      char res[36];

      const uint64_t x = betole64(ab);
      const uint64_t y = betole64(cd);

      u32tos(x >> 32, hex);
      memcpy(res, hex, 8);
      res[8] = '-';
      u32tos(x, hex);
      memcpy(res+9, hex, 4);
      res[13] = '-';
      memcpy(res+14, hex+4, 4);
      res[18] = '-';

      u32tos(y >> 32, hex);
      memcpy(res+19, hex, 4);
      res[23] = '-';
      memcpy(res+24, hex+4, 4);
      u32tos(y, hex);
      memcpy(res+28, hex, 8);

      return std::string(res, 36);
    }

    friend std::ostream& operator<< (std::ostream& stream, const UUID& uuid) {
      return stream << uuid.str();
    }

    friend std::istream& operator>> (std::istream& stream, UUID& uuid) {
      std::string s;
      stream >> s;
      uuid = fromStr(s);
      if( false ) {
        stream.setstate(std::ios::failbit);
      }
      return stream;
    }

  private:
    uint64_t ab;
    uint64_t cd;
};

template <typename RNG>
class UUIDGenerator {
  public:
    UUIDGenerator() : generator(new RNG(std::random_device()())), distribution(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max())
    {}

    UUIDGenerator(uint64_t seed) : generator(new RNG(seed)), distribution(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max())
    {}

    UUIDGenerator(const RNG &gen) : generator(gen), distribution(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max())
    {}

    UUID getUUID() {
      uint64_t ab = (distribution(*generator) & 0xFFFFFFFFFFFF0FFFull) | 0x0000000000004000ull;
      uint64_t cd = (distribution(*generator) & 0x3FFFFFFFFFFFFFFFull) | 0x8000000000000000ull;
      return UUID(ab, cd);
    }

  private:
    std::shared_ptr<RNG> generator;
    std::uniform_int_distribution<uint64_t> distribution;
};
