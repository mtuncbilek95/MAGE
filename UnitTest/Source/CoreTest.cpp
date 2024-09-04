#include <catch2/catch_all.hpp>

#include "Engine/Core/Core.h"

TEST_CASE("Primitive Values")
{
	REQUIRE(sizeof(i8) == 1);
	REQUIRE(sizeof(i16) == 2);
	REQUIRE(sizeof(i32) == 4);
	REQUIRE(sizeof(i64) == 8);

	REQUIRE(sizeof(u8) == 1);
	REQUIRE(sizeof(u16) == 2);
	REQUIRE(sizeof(u32) == 4);
	REQUIRE(sizeof(u64) == 8);

	REQUIRE(sizeof(f32) == 4);
	REQUIRE(sizeof(f64) == 8);
}