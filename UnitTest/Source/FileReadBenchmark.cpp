#include <catch2/catch_all.hpp>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>

#include "Engine/Platform/PlatformFile.h"
#include "Engine/Platform/PlatformPath.h"

namespace fs = std::filesystem;

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

std::string ReadFileInC(const std::string& filePath)
{
	FILE* file = std::fopen(filePath.c_str(), "r");
	if (!file)
	{
		perror("Error opening file");
		return {};
	}

	// Go to the end of the file to determine its size
	std::fseek(file, 0, SEEK_END);
	long fileSize = std::ftell(file);
	std::rewind(file);

	// Allocate memory to hold the entire file content
	std::string buffer;
	buffer.resize(fileSize);
	std::fread(&buffer[0], fileSize, sizeof(char), file);
	std::fclose(file);

	return buffer;
}

std::string ReadFileCPP(const Path& filePath) {
	if (!fs::exists(filePath)) {
		throw std::runtime_error("File does not exist: " + filePath.string());
	}

	std::ifstream file(filePath, std::ios::ate | std::ios::binary);
	if (!file) {
		throw std::runtime_error("Failed to open file: " + filePath.string());
	}

	std::streamsize fileSize = file.tellg();
	std::string content(fileSize, '\0');  // Directly allocate the required size
	file.seekg(0, std::ios::beg);

	if (!file.read(&content[0], fileSize)) {
		throw std::runtime_error("Error reading file: " + filePath.string());
	}

	return content;
}

std::string ReadFileOS(const Path& filePath)
{
	String result;
	if (!MAGE::PlatformFile::Read(filePath, result))
	{
		perror("Error opening file");
		return {};
	}

	return result;
}

TEST_CASE("Read File Benchmark")
{
	std::string test = TOSTRING(TEST_FOLDER_PATH);
	test += "/TestImage.jpg";

	BENCHMARK("Read file in C (fopen, fseek)")
	{
		return ReadFileInC(test);
	};

	BENCHMARK("Read file in C++ (stream)")
	{
		return ReadFileCPP(test);
	};

	BENCHMARK("Read file in OS (Win32 CreateFile)")
	{
		return ReadFileOS(test);
	};
}
