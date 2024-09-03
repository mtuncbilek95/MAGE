#include <catch2/catch_all.hpp>

#include "Engine/Containers/String.h"

TEST_CASE("String")
{
	SECTION("Constructor")
	{
		MAGE::String str("Hello, World!");
		REQUIRE(str.Data() != nullptr);
		REQUIRE(str.Size() == 13);
		REQUIRE(strcmp(str.Data(), "Hello, World!") == 0);
	}
}