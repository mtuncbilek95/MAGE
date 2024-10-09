#include "PlatformFile.h"

#if defined(DELUSION_WINDOWS)
#include <Windows.h>
#endif // DELUSION_WINDOWS

#include "Engine/ErrorHandler/ErrorChecker.h"

namespace MAGE
{
#if defined(DELUSION_WINDOWS)
	bool PlatformFile::Exists(const String& path)
	{
		// Check if the file exists
		DWORD attribute = GetFileAttributesA(path.c_str());
		return (attribute != INVALID_FILE_ATTRIBUTES && !(attribute & FILE_ATTRIBUTE_DIRECTORY));
	}

	bool PlatformFile::Create(const String& path, RawBuffer& buffer)
	{
		// Create the file
		HANDLE hFile = CreateFileA(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		// Check if the file was created
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		{
			// Log the error
			spdlog::warn("Failed to create the file: {}", ErrorChecker::Handler::GetErrorString());
			return false;
		}

		// Close the file
		CloseHandle(hFile);
		return true;
	}

	bool PlatformFile::Read(const String& path, OwnedBuffer& buffer, bool nullTerminated, const usize start, const usize end)
	{
		// Check if the file exists
		if (!PlatformFile::Exists(path))
		{
			spdlog::warn("Failed to read the file: File does not exist");
			return false;
		}

		// Open the file
		HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		{
			spdlog::warn("Failed to read the file: {}", ErrorChecker::Handler::GetErrorString());
			return false;
		}

		// Get the file size
		LARGE_INTEGER fileSize;
		if (!GetFileSizeEx(hFile, &fileSize))
		{
			spdlog::warn("Failed to read the file: {}", ErrorChecker::Handler::GetErrorString());
			CloseHandle(hFile);
			return false;
		}

		// Read the file
		DWORD bytesRead;
		char* data = new char[fileSize.QuadPart + (nullTerminated ? 1 : 0)];
		if (ReadFile(hFile, data, fileSize.QuadPart, &bytesRead, NULL) == 0)
		{
			spdlog::warn("Failed to read the file: {}", ErrorChecker::Handler::GetErrorString());
			CloseHandle(hFile);
			delete[] data;
			return false;
		}

		// Close the file
		CloseHandle(hFile);

		// Null terminate the buffer
		if (nullTerminated)
			data[fileSize.QuadPart] = '\0';

		// Set the output
		buffer = OwnedBuffer(data, fileSize.QuadPart);
		delete[] data;

		return true;
	}

	bool PlatformFile::Read(const String& path, String& buffer, const usize start, const usize end)
	{
		// Check if the file exists
		if (!PlatformFile::Exists(path))
		{
			spdlog::warn("Failed to read the file: File does not exist");
			return false;
		}

		// Open the file
		HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		{
			spdlog::warn("Failed to read the file: {}", ErrorChecker::Handler::GetErrorString());
			return false;
		}

		// Get the file size
		LARGE_INTEGER fileSize;
		if (!GetFileSizeEx(hFile, &fileSize))
		{
			spdlog::warn("Failed to read the file: {}", ErrorChecker::Handler::GetErrorString());
			CloseHandle(hFile);
			return false;
		}

		// Read the file
		DWORD bytesRead;
		buffer.resize(fileSize.QuadPart + 1);
		char* data = &buffer[0];
		if (ReadFile(hFile, data, fileSize.QuadPart, &bytesRead, NULL) == 0)
		{
			spdlog::warn("Failed to read the file: {}", ErrorChecker::Handler::GetErrorString());
			CloseHandle(hFile);
			return false;
		}

		// Close the file
		CloseHandle(hFile);
		buffer[fileSize.QuadPart] = '\0';

		return true;
	}

	bool PlatformFile::Write(const String& path, RawBuffer& buffer, const usize startOffset)
	{
		HANDLE hFile;

		// Check if the file exists
		if (PlatformFile::Exists(path))
			hFile = CreateFileA(path.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		else
			hFile = CreateFileA(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		// Check if the file was created
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
			return false;

		// Set the file pointer to the offset
		SetFilePointer(hFile, startOffset, NULL, FILE_BEGIN);

		// Write the data to the file
		DWORD bytesWritten;
		if (WriteFile(hFile, buffer.Data(), buffer.Size(), &bytesWritten, NULL) == 0)
		{
			// Log the error
			spdlog::warn("Failed to write to the file: {}", ErrorChecker::Handler::GetErrorString());

			CloseHandle(hFile);
			return false;
		}

		// Close the file
		CloseHandle(hFile);
		return true;
	}

	bool PlatformFile::Write(const String& path, const String& buffer, const usize startOffset)
	{
		HANDLE hFile;

		// Check if the file exists
		if (PlatformFile::Exists(path))
			hFile = CreateFileA(path.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		else
			hFile = CreateFileA(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		// Check if the file was created
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
			return false;

		// Set the file pointer to the offset
		SetFilePointer(hFile, startOffset, NULL, FILE_BEGIN);

		// Write the data to the file
		DWORD bytesWritten;
		if (WriteFile(hFile, buffer.c_str(), buffer.size(), &bytesWritten, NULL) == 0)
		{
			// Log the error
			spdlog::warn("Failed to write to the file: {}", ErrorChecker::Handler::GetErrorString());

			CloseHandle(hFile);
			return false;
		}

		// Close the file
		CloseHandle(hFile);
		return true;
	}

	bool PlatformFile::Rename(const String& oldPath, const String& newPath)
	{
		if (!PlatformFile::Exists(oldPath))
		{
			spdlog::warn("Failed to move the file: Source file does not exist");
			return false;
		}

		if (Exists(newPath))
		{
			spdlog::warn("Failed to move the file: Destination file already exists");
			return false;
		}

		if (MoveFileA(oldPath.c_str(), newPath.c_str()) == 0)
		{
			spdlog::warn("Failed to move the file: {}", ErrorChecker::Handler::GetErrorString());
			return false;
		}

		return true;
	}

	bool PlatformFile::Copy(const String& oldPath, const String& newPath)
	{
		if (!PlatformFile::Exists(oldPath))
		{
			spdlog::warn("Failed to copy the file: Source file does not exist");
			return false;
		}

		if (CopyFileA(oldPath.c_str(), newPath.c_str(), true) == 0)
		{
			spdlog::warn("Failed to copy the file: {}", ErrorChecker::Handler::GetErrorString());
			return false;
		}

		return true;
	}

	bool PlatformFile::Move(const String& oldPath, const String& newPath)
	{
		if (!PlatformFile::Exists(oldPath))
		{
			spdlog::warn("Failed to move the file: Source file does not exist");
			return false;
		}

		if (PlatformFile::Exists(newPath))
		{
			spdlog::warn("Failed to move the file: Destination file already exists");
			return false;
		}

		if (MoveFileA(oldPath.c_str(), newPath.c_str()) == 0)
		{
			spdlog::warn("Failed to move the file: {}", ErrorChecker::Handler::GetErrorString());
			return false;
		}

		return true;
	}

	bool PlatformFile::Delete(const String& path)
	{
		if (DeleteFileA(path.c_str()) == 0)
		{
			spdlog::warn("Failed to delete the file: {}", ErrorChecker::Handler::GetErrorString());
			return false;
		}
		return true;
	}
#else
	bool PlatformFile::Exists(const String& path)
	{
		return false;
	}

	bool PlatformFile::Create(const String& path, IOAlloc::RawBuffer& buffer)
	{
		return false;
	}

	bool PlatformFile::Read(const String& path, IOAlloc::MemoryOwnedBuffer& buffer, bool nullTerminated, const usize start, const usize end)
	{
		return false;
	}

	bool PlatformFile::Read(const String& path, String& buffer, const usize start, const usize end)
	{
		return false;
	}

	bool PlatformFile::Write(const String& path, IOAlloc::RawBuffer& buffer, const usize startOffset)
	{
		return false;
	}

	bool PlatformFile::Write(const String& path, const String& buffer, const usize startOffset)
	{
		return false;
	}

	bool PlatformFile::Rename(const String& oldPath, const String& newPath)
	{
		return false;
	}

	bool PlatformFile::Copy(const String& oldPath, const String& newPath)
	{
		return false;
	}

	bool PlatformFile::Move(const String& oldPath, const String& newPath)
	{
		return false;
	}

	bool PlatformFile::Delete(const String& path)
	{
		return false;
	}
#endif // DELUSION_WINDOWS
}
