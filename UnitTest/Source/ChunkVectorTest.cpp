#include <catch2/catch_all.hpp>

#include "Engine/Containers/ChunkVector.h"
#include "Engine/Core/StdFix.h"

using namespace MAGE;

struct Test
{
	u32 mValue;
	Test(u32 value) : mValue(value) {}
	Test() : mValue(0) {}
};

TEST_CASE("ChunkVector")
{
	SECTION("PushBack")
	{
		Base::ChunkVector<u32> vec;

		REQUIRE(vec.Size() == 0);
		REQUIRE(vec.Capacity() == 32);

		for (u32 i = 0; i < 100; ++i)
			vec.PushBack(i);

		REQUIRE(vec.Size() == 100);
	}

	SECTION("EmplaceBack")
	{
		Base::ChunkVector<Test> vec;

		REQUIRE(vec.Size() == 0);
		REQUIRE(vec.Capacity() == 32);

		for (u32 i = 0; i < 100; ++i)
			vec.EmplaceBack(i);

		REQUIRE(vec.Size() == 100);

		for (u32 i = 0; i < 100; ++i)
			REQUIRE(vec[i].mValue == i);
	}

	SECTION("PopBack")
	{
		Base::ChunkVector<u32> vec;

		REQUIRE(vec.Size() == 0);
		REQUIRE(vec.Capacity() == 32);

		for (u32 i = 0; i < 100; ++i)
			vec.PushBack(i);

		REQUIRE(vec.Size() == 100);

		u64 tempVal = vec.Capacity();

		for (u32 i = 0; i < 50; ++i)
			vec.PopBack();

		REQUIRE(vec.Size() == 50);
		REQUIRE(vec.Capacity() == tempVal);
	}

	SECTION("Clear")
	{
		Base::ChunkVector<u32> vec;

		REQUIRE(vec.Size() == 0);
		REQUIRE(vec.Capacity() == 32);

		for (u32 i = 0; i < 100; ++i)
			vec.PushBack(i);

		REQUIRE(vec.Size() == 100);

		vec.Clear();

		REQUIRE(vec.Size() == 0);
		REQUIRE(vec.Capacity() == 32);
	}

	SECTION("Add/Fill")
	{
		Base::ChunkVector<String> vec;

		REQUIRE(vec.Size() == 0);
		REQUIRE(vec.Capacity() == 32);

		for (u32 i = 0; i < 100; ++i)
			vec.PushBack("a");

		REQUIRE(vec.Size() == 100);

		vec.Remove(56);
		vec.Add("z");

		REQUIRE(vec.Size() == 100);
		REQUIRE(vec[56] == "z");
	}
}