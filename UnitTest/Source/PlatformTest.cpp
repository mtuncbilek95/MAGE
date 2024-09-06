#include <catch2/catch_all.hpp>

#include "Engine/Platform/PlatformDirectory.h"
#include "Engine/Platform/PlatformFile.h"
#include "Engine/Platform/PlatformHardware.h"
#include "Engine/Platform/PlatformPath.h"

using namespace MAGE;

TEST_CASE("Platform Path")
{
	REQUIRE(PlatformPath::GetExecPath().size() > 0);
	REQUIRE(PlatformPath::GetProgramFilesPath().size() > 0);

#if defined(MAGE_WINDOWS)
	REQUIRE(PlatformPath::GetProgramFilesPath().compare("C:/Program Files") == 0);
#endif

	REQUIRE(PlatformPath::GetAppDataPath().size() > 0);
	REQUIRE(PlatformPath::GetTemporaryPath().size() > 0);
	REQUIRE(PlatformPath::GetHomePath().size() > 0);
}

TEST_CASE("Platform Directory")
{
	REQUIRE(PlatformDirectory::Create("UnitTest"));
	REQUIRE(PlatformDirectory::Exists("UnitTest"));
	REQUIRE(PlatformDirectory::Delete("UnitTest"));
	REQUIRE(!PlatformDirectory::Exists("UnitTest"));
}

TEST_CASE("Platform File")
{
}

TEST_CASE("Platform Hardware")
{
	REQUIRE(PlatformHardware::GetCPUCores() > 0);
	REQUIRE(PlatformHardware::GetCPUThreads() > 0);
	REQUIRE(PlatformHardware::GetHostMemorySize() > 0);
	REQUIRE(PlatformHardware::GetCPUName().size() > 0);
	REQUIRE(PlatformHardware::GetGPUName().size() > 0);
}