#include <catch2/catch_all.hpp>

#include "Engine/Containers/ChunkVector.h"

using namespace MAGE;

TEST_CASE("ChunkVector")
{
	Base::ChunkVector<u32> vec;

	REQUIRE(vec.Size() == 0);
	REQUIRE(vec.Capacity() == 32);

	vec.PushBack(1);
	vec.PushBack(2);
	vec.PushBack(3);

	REQUIRE(vec.Size() == 3);


	printf("Size: %llu\n", vec.Size());
	printf("Capacity: %llu\n", vec.Capacity());
	printf("Data: ");
	for (u32 i = 0; i < vec.Size(); ++i)
	{
		printf("%d\n", vec[i]);
	}
}