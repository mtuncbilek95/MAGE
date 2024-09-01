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