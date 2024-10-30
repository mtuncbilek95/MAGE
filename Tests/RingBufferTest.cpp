#include <catch2/catch_all.hpp>

#include <Engine/Data/Containers/RingBuffer.h>

using namespace MAGE;

TEST_CASE("Init Constructor", "[RingBuffer]")
{
	RingBuffer<String> buffer;

	REQUIRE(buffer.Size() == 0);
	REQUIRE(buffer.Capacity() == 1000);
}

TEST_CASE("Push Val", "[RingBuffer]")
{
	RingBuffer<String> buffer;

	for (i32 i = 0; i < 400; i++)
		buffer.Push(std::to_string(i));

	REQUIRE(buffer.Size() == 400);
}

TEST_CASE("Iterator", "[RingBuffer]")
{
	RingBuffer<String> buffer;

	for (i32 i = 0; i < 400; i++)
		buffer.Push(std::to_string(i));

	i32 counterIndex = 0;
	for (auto& it : buffer)
		REQUIRE(strcmp(it.CharString(), std::to_string(counterIndex).c_str()) == 0);
}
