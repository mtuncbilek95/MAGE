#pragma once

#if defined(MAGE_WINDOWS)
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64, s64;

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

typedef float f32;
typedef double f64;

typedef bool b8;
#endif

constexpr u64 u64_max = 0xFFFFFFFFFFFFFFFF;
constexpr u32 u32_max = 0xFFFFFFFF;
constexpr u16 u16_max = 0xFFFF;
constexpr u8 u8_max = 0xFF;

#define MB_TO_BYTE(mb) (mb * 1024.0f * 1024.0f)
#define KB_TO_BYTE(kb) (kb * 1024.0f)
#define BYTE_TO_MB(bt) (bt / 1024.0f / 1024.0f)
#define BYTE_TO_KB(bt) (bt / 1024.0f)

#define GENERATE_ENUM_FLAG(EnumType, PrimitiveType) \
    inline EnumType operator | (EnumType a, EnumType b) \
    { \
        return static_cast<EnumType>(static_cast<PrimitiveType>(a) | static_cast<PrimitiveType>(b)); \
    } \
    inline b8 operator & (EnumType a, EnumType b) \
    { \
        return (static_cast<PrimitiveType>(a) & static_cast<PrimitiveType>(b)) != 0; \
    }
