#include "SharedFileHelper.h"

#include "Engine/Platform/PlatformFile.h"
#include "Engine/Platform/PlatformErrorMessage.h"

#include <stdexcept>
#include <spdlog/spdlog.h>

#if defined(MAGE_WINDOWS)
#include <Windows.h>
#endif

namespace MAGE
{
#if defined(MAGE_WINDOWS)
	SharedFileHelper::SharedFileHelper(const String& filePath, s64 initialSize) : mMemoryMappedFile(nullptr), mWritten(0)
	{
		DWORD desiredMode = OPEN_ALWAYS;

		// If the file doesn't exist, we need to create it
		if (!PlatformFile::Exists(filePath))
			desiredMode = CREATE_ALWAYS;

		// Creeate or open the file depending on the desired mode
		auto fileHandler = CreateFileA(filePath.c_str(), GENERIC_ALL, NULL, NULL, desiredMode, FILE_ATTRIBUTE_NORMAL, NULL);
		if (fileHandler == INVALID_HANDLE_VALUE || fileHandler == NULL)
			throw std::runtime_error("Failed to open file" + PlatformErrorMessage::GetLastKnownError());

		// Get the file size
		LARGE_INTEGER fileSize{};
		if (!GetFileSizeEx(fileHandler, &fileSize))
			throw std::runtime_error("Failed to get file size: " + PlatformErrorMessage::GetLastKnownError());

		// Check the coalation of the file size and the initial size
		if (fileSize.QuadPart <= 0 && initialSize == 0)
			throw std::runtime_error("File size is 0 and initial size is 0. Both cannot be at the same time.");

		// Set the file size to the initial size
		if (initialSize > 0)
		{
			if (!SetFilePointer(fileHandler, initialSize, NULL, FILE_BEGIN))
				throw std::runtime_error("Failed to set file pointer: " + PlatformErrorMessage::GetLastKnownError());

			if (!SetEndOfFile(fileHandler))
				throw std::runtime_error("Failed to set end of file: " + PlatformErrorMessage::GetLastKnownError());
		}

		if (fileHandler)
			CloseHandle(fileHandler);

		mMemoryMappedFile = new MemoryMappedFile(filePath);
	}

	SharedFileHelper::~SharedFileHelper()
	{
		if (mMemoryMappedFile)
			delete mMemoryMappedFile;
	}

	void SharedFileHelper::Resize(s64 newSize)
	{
		mMemoryMappedFile->mFileSize = newSize;

		// Unmap the mapData
		if (!(mMemoryMappedFile->mFileMapping == NULL) && !(mMemoryMappedFile->mFileMapping == INVALID_HANDLE_VALUE))
			UnmapViewOfFile(mMemoryMappedFile->mMemData);

		// Close the file mapping
		if (mMemoryMappedFile->mFileMapping)
			CloseHandle(mMemoryMappedFile->mFileMapping);

		// Resize the core file
		if (!SetFilePointer(mMemoryMappedFile->mFileHandle, DWORD(mMemoryMappedFile->mFileSize), NULL, FILE_BEGIN))
			throw std::runtime_error(PlatformErrorMessage::GetLastKnownError());

		if (!SetEndOfFile(mMemoryMappedFile->mFileHandle))
			throw std::runtime_error(PlatformErrorMessage::GetLastKnownError());

		// Create file mapping
		mMemoryMappedFile->mFileMapping = CreateFileMappingW(mMemoryMappedFile->mFileHandle, NULL, PAGE_READWRITE, 0, 0, NULL);
		if (mMemoryMappedFile->mFileMapping == NULL || mMemoryMappedFile->mFileMapping == INVALID_HANDLE_VALUE)
			throw std::runtime_error(PlatformErrorMessage::GetLastKnownError());

		if (mMemoryMappedFile->mFileMapping)
		{
			// Map the file to memory
			mMemoryMappedFile->mMemData = MapViewOfFile(mMemoryMappedFile->mFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if (mMemoryMappedFile->mMemData == NULL || mMemoryMappedFile->mMemData == INVALID_HANDLE_VALUE)
				throw std::runtime_error(PlatformErrorMessage::GetLastKnownError());
		}
	}

	void SharedFileHelper::Trim(s64 newSize)
	{
		if (newSize >= mMemoryMappedFile->mFileSize)
			return;

		mMemoryMappedFile->mFileSize = newSize;

		// Unmap the mapData
		if (!(mMemoryMappedFile->mFileMapping == NULL) && !(mMemoryMappedFile->mFileMapping == INVALID_HANDLE_VALUE))
			UnmapViewOfFile(mMemoryMappedFile->mMemData);

		// Close the file mapping
		if (mMemoryMappedFile->mFileMapping)
			CloseHandle(mMemoryMappedFile->mFileMapping);

		// Resize the core file
		if (!SetFilePointer(mMemoryMappedFile->mFileHandle, DWORD(mMemoryMappedFile->mFileSize), NULL, FILE_BEGIN))
			throw std::runtime_error(PlatformErrorMessage::GetLastKnownError());

		if (!SetEndOfFile(mMemoryMappedFile->mFileHandle))
			throw std::runtime_error(PlatformErrorMessage::GetLastKnownError());

		// Create file mapping
		mMemoryMappedFile->mFileMapping = CreateFileMappingW(mMemoryMappedFile->mFileHandle, NULL, PAGE_READWRITE, 0, 0, NULL);
		if (mMemoryMappedFile->mFileMapping == NULL || mMemoryMappedFile->mFileMapping == INVALID_HANDLE_VALUE)
			throw std::runtime_error(PlatformErrorMessage::GetLastKnownError());

		if (mMemoryMappedFile->mFileMapping)
		{
			// Map the file to memory
			mMemoryMappedFile->mMemData = MapViewOfFile(mMemoryMappedFile->mFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if (mMemoryMappedFile->mMemData == NULL || mMemoryMappedFile->mMemData == INVALID_HANDLE_VALUE)
				throw std::runtime_error(PlatformErrorMessage::GetLastKnownError());
		}
	}
#endif

	void SharedFileHelper::Write(const String& data)
	{
		// if the sum of already written size and the new data's size is bigger than current size, resize the file
		if (mWritten + data.size() > mMemoryMappedFile->mFileSize)
			Resize(mMemoryMappedFile->mFileSize + data.size());

		// Copy the data to the memory
		memcpy((char*)mMemoryMappedFile->mMemData + mWritten, data.c_str(), data.size());
		mWritten += data.size();
	}

	void SharedFileHelper::MoveOffset(i64 offset)
	{
		mWritten += offset;
	}

	bool SharedFileHelper::CopyTo(void* dest, s64 size, s64 offset)
	{
		if (offset + size > mMemoryMappedFile->mFileSize)
			return false;

		memcpy(dest, (char*)mMemoryMappedFile->mMemData + offset, size);
		return true;
	}

	bool SharedFileHelper::CloseFile()
	{
		return mMemoryMappedFile->CloseFile();
	}
}
