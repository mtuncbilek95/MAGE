#include <catch2/catch_all.hpp>

#include <Engine/Platform/PlatformFile.h>
#include <Engine/Memory/MemoryBuffer.h>
#include <Engine/Memory/MemoryOwnedBuffer.h>
#include <Engine/Platform/PlatformDirectory.h>
#include <Engine/Platform/PlatformPath.h>

using namespace MAGE;

#if defined(MAGE_WINDOWS)
TEST_CASE("Windows") {
    printf("Detected\n");
}
#endif

#if defined(MAGE_LINUX)
TEST_CASE("Linux")
{
  printf("Detected\n");
}
#endif

TEST_CASE("PlatformFile") {
    if (PlatformFile::Exists("UnitTestObj.txt"))
        PlatformFile::Delete("UnitTestObj.txt");

    SECTION("Create/Exists/Delete") {
        const String path = "UnitTestObj.txt";
        REQUIRE(PlatformFile::Exists(path) == false);
        REQUIRE(PlatformFile::Create(path) == true);
        REQUIRE(PlatformFile::Exists(path) == true);
        REQUIRE(PlatformFile::Delete(path) == true);
        REQUIRE(PlatformFile::Exists(path) == false);

        printf("Create/Exists/Delete tested\n");
    }

    SECTION("Write/Read (String)") {
        const String path = "UnitTestObj.txt";
        const String data = "Hello, World!";
        String readData;

        REQUIRE(PlatformFile::Write(path, data) == true);
        REQUIRE(PlatformFile::Exists(path) == true);
        REQUIRE(PlatformFile::Read(path, readData));
        REQUIRE(data == readData);
        REQUIRE(PlatformFile::Delete(path) == true);
        REQUIRE(PlatformFile::Exists(path) == false);
    }

    SECTION("Write/Read (MemoryBuffer)") {
        const String path = "UnitTestObj.txt";
        const String data = "Hello, World!";
        const MemoryBuffer buffer((void *) data.data(), data.size());
        MemoryOwnedBuffer readBuffer;

        REQUIRE(PlatformFile::Write(path, buffer) == true);
        REQUIRE(PlatformFile::Exists(path) == true);
        REQUIRE(PlatformFile::Read(path, readBuffer));
        String readData((const char *) readBuffer.GetData(), readBuffer.GetSize());
        REQUIRE(data == readData);
        REQUIRE(PlatformFile::Delete(path) == true);
        REQUIRE(PlatformFile::Exists(path) == false);
    }
}

TEST_CASE("PlatformPath") {
    if (PlatformDirectory::Exists("UnitTestDir"))
        PlatformDirectory::Delete("UnitTestDir");

    SECTION("ProgramFiles") {
        const String path = PlatformPath::GetProgramFilesDir();
        REQUIRE(PlatformDirectory::Exists(path) == true);
    }

    SECTION("AppData") {
        const String path = PlatformPath::GetAppDataDir();
        REQUIRE(PlatformDirectory::Exists(path) == true);
    }

    SECTION("Temp") {
        const String path = PlatformPath::GetTempDir();
        REQUIRE(PlatformDirectory::Exists(path) == true);
    }
}

TEST_CASE("PlatformDirectory") {
    if (PlatformDirectory::Exists("UnitTestDir"))
        PlatformDirectory::Delete("UnitTestDir");

    SECTION("Create/Exists/Delete") {
        const String path = "UnitTestDir";
        REQUIRE(PlatformDirectory::Exists(path) == false);
        REQUIRE(PlatformDirectory::Create(path) == true);
        REQUIRE(PlatformDirectory::Exists(path) == true);
        REQUIRE(PlatformDirectory::Delete(path) == true);
        REQUIRE(PlatformDirectory::Exists(path) == false);
    }
}
