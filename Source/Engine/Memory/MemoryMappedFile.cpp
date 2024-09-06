#include "MemoryMappedFile.h"

#include "Engine/Platform/PlatformErrorMessage.h"

#include <stdexcept>
#include <spdlog/spdlog.h>

namespace MAGE
{
#if defined(MAGE_WINDOWS)
	MemoryMappedFile::MemoryMappedFile(const String& filePath) : mMemData(nullptr), mFileSize(0), mFileHandle(nullptr), mFileMapping(nullptr)
	{
		mFileHandle = CreateFileA(filePath.c_str(), GENERIC_ALL, FILE_SHARE_WRITE,
			NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (mFileHandle == INVALID_HANDLE_VALUE || mFileHandle == NULL)
			throw std::runtime_error("Failed to open file: " + PlatformErrorMessage::GetLastKnownError());

		LARGE_INTEGER fileSize{};
		if(!GetFileSizeEx(mFileHandle, &fileSize))
			throw std::runtime_error("Failed to get file size: " + PlatformErrorMessage::GetLastKnownError());

		mFileSize = fileSize.QuadPart;

		mFileMapping = CreateFileMappingA(mFileHandle, NULL, PAGE_READWRITE, 0, 0, NULL);
		if (mFileMapping == INVALID_HANDLE_VALUE || mFileMapping == NULL)
			throw std::runtime_error("Failed to create file mapping: " + PlatformErrorMessage::GetLastKnownError());

		mMemData = MapViewOfFile(mFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (mMemData == INVALID_HANDLE_VALUE || mMemData == NULL)
			throw std::runtime_error("Failed to map view of file: " + PlatformErrorMessage::GetLastKnownError());
	}

	MemoryMappedFile::~MemoryMappedFile()
	{
		if (mMemData)
			UnmapViewOfFile(mMemData);
		if (mFileMapping)
			CloseHandle(mFileMapping);
		if (mFileHandle)
			CloseHandle(mFileHandle);
	}

	bool MemoryMappedFile::CloseFile()
	{
		if (mMemData)
		{
			if (!UnmapViewOfFile(mMemData))
			{
				spdlog::error("Failed to unmap view of file: {}", PlatformErrorMessage::GetLastKnownError());
				return false;
			}
		}

		if (mFileMapping)
		{
			if (!CloseHandle(mFileMapping))
			{
				spdlog::error("Failed to close file mapping: {}", PlatformErrorMessage::GetLastKnownError());
				return false;
			}
		}

		if (mFileHandle)
		{
			if (!CloseHandle(mFileHandle))
			{
				spdlog::error("Failed to close file handle: {}", PlatformErrorMessage::GetLastKnownError());
				return false;
			}
		}

		return true;
	}
#endif
}
