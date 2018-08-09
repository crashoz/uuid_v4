#pragma once

#if defined(BIGENDIAN)
  // Try to use compiler intrinsics
  #if defined(__GNUC__) // GCC and CLANG
    #define betole16(x) __builtin_bswap16(x)
    #define betole32(x) __builtin_bswap32(x)
    #define betole64(x) __builtin_bswap64(x)
  #elif defined(_MSC_VER) // MSVC
    #include <stdlib.h>
    #define betole16(x) _byteswap_ushort(x)
    #define betole32(x) _byteswap_ulong(x)
    #define betole64(x) _byteswap_uint64(x)
  #else
    #define FALLBACK_SWAP
    #define betole16(x) swap_u16(x)
    #define betole32(x) swap_u32(x)
    #define betole64(x) swap_u64(x)
  #endif
#else
  #define betole16(x) (x)
  #define betole32(x) (x)
  #define betole64(x) (x)
#endif // BIGENDIAN

#if defined(FALLBACK_SWAP)
  #include <stdint.h>
  inline void swap_u16(uint16_t value)
  {
    return
        ((value & 0xFF00u) >>  8u) |
        ((value & 0x00FFu) <<  8u);
  }
  inline void swap_u32(uint32_t value)
  {
    return
        ((value & 0xFF000000u) >> 24u) |
        ((value & 0x00FF0000u) >>  8u) |
        ((value & 0x0000FF00u) <<  8u) |
        ((value & 0x000000FFu) << 24u);
  }
  inline void swap_u64(uint64_t value)
  {
    return
        ((value & 0xFF00000000000000u) >> 56u) |
        ((value & 0x00FF000000000000u) >> 40u) |
        ((value & 0x0000FF0000000000u) >> 24u) |
        ((value & 0x000000FF00000000u) >>  8u) |
        ((value & 0x00000000FF000000u) <<  8u) |
        ((value & 0x0000000000FF0000u) << 24u) |
        ((value & 0x000000000000FF00u) << 40u) |
        ((value & 0x00000000000000FFu) << 56u);
  }
#endif // FALLBACK_SWAP
