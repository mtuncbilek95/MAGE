#pragma once

#if defined(_MSC_VER) && _MSC_VER >= 1910

#if defined(_WIN64)
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

#define u64_max 0xFFFFFFFFFFFFFFFF
#define u32_max 0xFFFFFFFF
#define u16_max 0xFFFF
#define u8_max 0xFF
#endif

#define MB_TO_BYTE(mb) (mb * 1024.0f * 1024.0f)
#define KB_TO_BYTE(kb) (kb * 1024.0f)
#define BYTE_TO_MB(bt) (bt / 1024.0f / 1024.0f)
#define BYTE_TO_KB(bt) (bt / 1024.0f)

#define GENERATE_ENUM_FLAG(EnumType, primitiveType) \
FORCEINLINE static EnumType operator | (EnumType a, EnumType b) { return static_cast<EnumType>(static_cast<primitiveType>(a) | static_cast<primitiveType>(b)); } \
FORCEINLINE static b8 operator & (EnumType a, EnumType b) { return static_cast<b8>(static_cast<primitiveType>(a) & static_cast<primitiveType>(b)); }




