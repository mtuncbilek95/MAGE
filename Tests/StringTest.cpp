#include <catch2/catch_all.hpp>

#include <Engine/Data/Containers/String.h>

using namespace MAGE;

TEST_CASE("Init Constructor", "[String]")
{
	String str1;
	REQUIRE(str1 == "");

	String str2("Hello");
	REQUIRE(str2 == "Hello");

	String str3(std::string("World"));
	REQUIRE(str3 == "World");

	String str4(str2);
	REQUIRE(str4 == "Hello");

	String str5(std::move(str3));
	REQUIRE(str5 == "World");
}

TEST_CASE("Assignment", "[String]")
{
	String str1;
	str1 = "Hello";
	REQUIRE(str1 == "Hello");

	String str2;
	str2 = std::string("World");
	REQUIRE(str2 == "World");

	String str3;
	str3 = str1;
	REQUIRE(str3 == "Hello");

	String str4;
	str4 = std::move(str2);
	REQUIRE(str4 == "World");
}

TEST_CASE("Concatenation", "[String]")
{
	String str1("Hello");
	str1 += " World";
	REQUIRE(str1 == "Hello World");

	String str2("Hello");
	String str3 = str2 + " World";
	REQUIRE(str3 == "Hello World");

	String str4("Hello");
	str4 += std::string(" World");
	REQUIRE(str4 == "Hello World");

	String str5("Hello");
	String str6 = str5 + std::string(" World");
	REQUIRE(str6 == "Hello World");

	String str7("Hello");
	str7 += String(" World");
	REQUIRE(str7 == "Hello World");

	String str8("Hello");
	String str9 = str8 + String(" World");
	REQUIRE(str9 == "Hello World");

	String str10 = String("Hello") + " World";
	REQUIRE(str10 == "Hello World");

	String str11 = String("Hello") + std::string(" World");
	REQUIRE(str11 == "Hello World");

	String str12 = std::string("Hello") + String(" World");
	REQUIRE(str12 == "Hello World");
}

TEST_CASE("Comparison", "[String]")
{
	String str1("Hello");
	REQUIRE(str1 == "Hello");
	REQUIRE(str1 != "World");

	String str2("World");
	REQUIRE(str2 == "World");
	REQUIRE(str2 != "Hello");
}

TEST_CASE("Trim", "[String]")
{
	String str1("  Hello World  ");
	REQUIRE(str1.Trim() == "Hello World");
}

TEST_CASE("FindIndex", "[String]")
{
	String str1("Georg will save us all from our doom");
	REQUIRE(str1.FindStartIndexOf("save") == 11);
}

TEST_CASE("Upper/Lower", "[String]")
{
	String str1("Hello World");
	str1.ToUpper();
	REQUIRE(str1 == "HELLO WORLD");

	String str2("Hello World");
	str2.ToLower();
	REQUIRE(str2 == "hello world");
}

TEST_CASE("Substring", "[String]")
{
	String str1("Hello World");
	REQUIRE(str1.SubString(0, 5) == "Hello");
	REQUIRE(str1.SubString(6, 11) == "World");
}

TEST_CASE("StartsWith", "[String]")
{
	String str1("Hello World");
	REQUIRE(str1.StartsWith("Hello"));
	REQUIRE(!str1.StartsWith("World"));
}

TEST_CASE("EndsWith", "[String]")
{
	String str1("Hello World");
	REQUIRE(str1.EndsWith("World"));
	REQUIRE(!str1.EndsWith("Hello"));
}

TEST_CASE("Contains", "[String]")
{
	String str1("Hello World");
	REQUIRE(str1.Contains("Hello"));
	REQUIRE(str1.Contains("World"));
	REQUIRE(!str1.Contains("Georg"));
}

TEST_CASE("FindEndIndexOf", "[String]")
{
	String str1("Georg will save us all from our doom");
	REQUIRE(str1.FindEndIndexOf("save") == 11);
}

TEST_CASE("TrimFront", "[String]")
{
	String str1("  Hello World");
	REQUIRE(str1.TrimFront() == "Hello World");
}

TEST_CASE("TrimBack", "[String]")
{
	String str1("Hello World  ");
	REQUIRE(str1.TrimBack() == "Hello World");
}

TEST_CASE("Empty", "[String]")
{
	String str1;
	REQUIRE(str1.Empty());
	REQUIRE(!str1);

	String str2("Hello World");
	REQUIRE(!str2.Empty());
	REQUIRE(!!str2);
}

TEST_CASE("Size", "[String]")
{
	String str1("Hello World");
	REQUIRE(str1.Size() == 11);
}

TEST_CASE("Capacity", "[String]")
{
	String str1("Hello World");
	REQUIRE(str1.Capacity() >= 11);
}

TEST_CASE("PopBack", "[String]")
{
	String str1("Hello World");
	str1.PopBack();
	REQUIRE(str1 == "Hello Worl");
}

TEST_CASE("Clear", "[String]")
{
	String str1("Hello World");
	str1.Clear();
	REQUIRE(str1.Empty());
}

TEST_CASE("Split", "[String]")
{
	String str1("Hello,World");
	auto split = str1.Split(",");
	REQUIRE(split.size() == 2);
	REQUIRE(split[0] == "Hello");
	REQUIRE(split[1] == "World");
}

TEST_CASE("SplitAtLastOccurrence", "[String]")
{
	String str1 = "Hello,World,Georg";
	auto split = str1.SplitAtLastOccurrence(",");
	REQUIRE(split.first == "Hello,World");
	REQUIRE(split.second == "Georg");
}

TEST_CASE("SplitAtFirstOccurrence", "[String]")
{
	String str1 = "Hello,World,Georg";
	auto split = str1.SplitAtFirstOccurrence(",");
	REQUIRE(split.first == "Hello");
	REQUIRE(split.second == "World,Georg");
}

TEST_CASE("SplitAsStringViews", "[String]")
{
	String str1("Hello,World");
	auto split = str1.SplitAsStringViews(",");
	REQUIRE(split.size() == 2);
	REQUIRE(split[0] == "Hello");
	REQUIRE(split[1] == "World");
}

TEST_CASE("SplitAtFirstOccurrenceAsStringViews", "[String]")
{
	String str1 = "Hello,World,Georg";
	auto split = str1.SplitAtFirstOccurenceAsStringViews(",");
	REQUIRE(split.first == "Hello");
	REQUIRE(split.second == "World,Georg");
}

TEST_CASE("SplitAtLastOccurenceAsStringViews", "[String]")
{
	String str1 = "Hello,World,Georg";
	auto split = str1.SplitAtLastOccurenceAsStringViews(",");
	REQUIRE(split.first == "Hello,World");
	REQUIRE(split.second == "Georg");
}