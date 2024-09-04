#include <catch2/catch_all.hpp>

#include "Engine/Containers/ChunkVector.h"

using namespace MAGE;

TEST_CASE("ChunkVector")
{
	Base::ChunkVector<u32> vec;

	REQUIRE(vec.Size() == 0);

}