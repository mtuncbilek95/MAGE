/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include <cstdint>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef size_t usize;

typedef float f32;
typedef double f64;

typedef bool b8;

constexpr u64 u64_max = 0xFFFFFFFFFFFFFFFF;
constexpr u32 u32_max = 0xFFFFFFFF;
constexpr u16 u16_max = 0xFFFF;
constexpr u8 u8_max = 0xFF;

#include <type_traits>
#include <concepts>

template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;
