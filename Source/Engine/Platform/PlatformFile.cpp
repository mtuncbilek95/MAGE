#include "PlatformFile.h"

#if defined(MAGE_WINDOWS)
#include <Windows.h>
#endif

#include <spdlog/spdlog.h>

namespace MAGE
{
#if defined(MAGE_WINDOWS)
	static String GetLastErrorMessage()
	{
		// Get the error message ID, if any.
		DWORD errorID = GetLastError();
		if (errorID == 0)
			return String();

		LPSTR messageBuffer = nullptr;
		// Format the error message
		u64 size = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&messageBuffer, 0, NULL);

		// Buffer it up
		String message(messageBuffer, size);

		// Free the buffer and return the message
		LocalFree(messageBuffer);
		return message;
	}

	b8 PlatformFile::Exists(const Path& path)
	{
		// Check if the file exists
		DWORD attribute = GetFileAttributesA(path.string().c_str());
		return (attribute != INVALID_FILE_ATTRIBUTES && !(attribute & FILE_ATTRIBUTE_DIRECTORY));
	}

	b8 PlatformFile::Create(const Path& path)
	{
		// Create the file
		HANDLE hFile = CreateFileA(path.string().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		// Check if the file was created
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		{
			// Log the error
			spdlog::warn("Failed to create the file: {}", GetLastErrorMessage());
			return false;
		}

		// Close the file
		CloseHandle(hFile);
		return true;
	}

	b8 PlatformFile::Delete(const Path& path)
	{
		if (DeleteFileA(path.string().c_str()) == 0)
		{
			spdlog::warn("Failed to delete the file: {}", GetLastErrorMessage());
			return false;
		}
		return true;
	}

	b8 PlatformFile::Copy(const Path& source, const Path& destination, bool bOverwrite)
	{
		if (!Exists(source))
		{
			spdlog::warn("Failed to copy the file: Source file does not exist");
			return false;
		}

		if (Exists(destination) && !bOverwrite)
		{
			spdlog::warn("Failed to copy the file: Destination file already exists");
			return false;
		}

		if (CopyFileA(source.string().c_str(), destination.string().c_str(), bOverwrite) == 0)
		{
			spdlog::warn("Failed to copy the file: {}", GetLastErrorMessage());
			return false;
		}

		return true;
	}

	b8 PlatformFile::Move(const Path& source, const Path& destination)
	{
		if (!Exists(source))
		{
			spdlog::warn("Failed to move the file: Source file does not exist");
			return false;
		}

		if (Exists(destination))
		{
			spdlog::warn("Failed to move the file: Destination file already exists");
			return false;
		}

		if (MoveFileA(source.string().c_str(), destination.string().c_str()) == 0)
		{
			spdlog::warn("Failed to move the file: {}", GetLastErrorMessage());
			return false;
		}

		return true;
	}

	b8 PlatformFile::Rename(const Path& source, const Path& destination)
	{
		if (!Exists(source))
		{
			spdlog::warn("Failed to rename the file: Source file does not exist");
			return false;
		}

		if (Exists(destination))
		{
			spdlog::warn("Failed to rename the file: The named file already exists");
			return false;
		}

		if (MoveFileA(source.string().c_str(), destination.string().c_str()) == 0)
		{
			spdlog::warn("Failed to rename the file: {}", GetLastErrorMessage());
			return false;
		}

		return true;
	}

	b8 PlatformFile::Read(const Path& path, String& outFile, bool nullTerminate)
	{
		// Check if the file exists
		if (!Exists(path))
		{
			spdlog::warn("Failed to read the file: File does not exist");
			return false;
		}

		// Open the file
		HANDLE hFile = CreateFileA(path.string().c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		{
			spdlog::warn("Failed to read the file: {}", GetLastErrorMessage());
			return false;
		}

		// Get the file size
		DWORD fileSize = GetFileSize(hFile, NULL);
		if (fileSize == INVALID_FILE_SIZE)
		{
			spdlog::warn("Failed to read the file: {}", GetLastErrorMessage());
			CloseHandle(hFile);
			return false;
		}

		// Read the file
		DWORD bytesRead;
		char* buffer = new char[fileSize + (nullTerminate ? 1 : 0)];
		if (ReadFile(hFile, buffer, fileSize, &bytesRead, NULL) == 0)
		{
			spdlog::warn("Failed to read the file: {}", GetLastErrorMessage());
			CloseHandle(hFile);
			delete[] buffer;
			return false;
		}

		// Close the file
		CloseHandle(hFile);

		// Null terminate the buffer
		if (nullTerminate)
			buffer[fileSize] = '\0';

		// Set the output
		outFile = String(buffer, fileSize);
		delete[] buffer;

		return true;
	}

	b8 PlatformFile::Read(const Path& path, MemoryOwnedBuffer& outFile, bool nullTerminate)
	{
		// Check if the file exists
		if (!Exists(path))
		{
			spdlog::warn("Failed to read the file: File does not exist");
			return false;
		}

		// Open the file
		HANDLE hFile = CreateFileA(path.string().c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		{
			spdlog::warn("Failed to read the file: {}", GetLastErrorMessage());
			return false;
		}

		// Get the file size
		DWORD fileSize = GetFileSize(hFile, NULL);
		if (fileSize == INVALID_FILE_SIZE)
		{
			spdlog::warn("Failed to read the file: {}", GetLastErrorMessage());
			CloseHandle(hFile);
			return false;
		}

		// Read the file
		DWORD bytesRead;
		char* buffer = new char[fileSize + (nullTerminate ? 1 : 0)];
		if (ReadFile(hFile, buffer, fileSize, &bytesRead, NULL) == 0)
		{
			spdlog::warn("Failed to read the file: {}", GetLastErrorMessage());
			CloseHandle(hFile);
			delete[] buffer;
			return false;
		}

		// Close the file
		CloseHandle(hFile);

		// Null terminate the buffer
		if (nullTerminate)
			buffer[fileSize] = '\0';

		// Set the output
		outFile = MemoryOwnedBuffer(buffer, fileSize);
		delete[] buffer;

		return true;
	}

	b8 PlatformFile::Write(const Path& path, const String& data, const u64 offset)
	{
		HANDLE hFile;

		// Check if the file exists
		if (Exists(path))
			hFile = CreateFileA(path.string().c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		else
			hFile = CreateFileA(path.string().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		// Check if the file was created
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
			return false;

		// Set the file pointer to the offset
		SetFilePointer(hFile, offset, NULL, FILE_BEGIN);

		// Write the data to the file
		DWORD bytesWritten;
		if (WriteFile(hFile, data.c_str(), data.size(), &bytesWritten, NULL) == 0)
		{
			// Log the error
			spdlog::warn("Failed to write to the file: {}", GetLastErrorMessage());

			CloseHandle(hFile);
			return false;
		}

		// Close the file
		CloseHandle(hFile);
		return true;
	}

	b8 PlatformFile::Write(const Path& path, const MemoryBuffer& data, const u64 offset)
	{
		HANDLE hFile;

		// Check if the file exists
		if (Exists(path))
			hFile = CreateFileA(path.string().c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		else
			hFile = CreateFileA(path.string().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		// Check if the file was created
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
			return false;

		// Set the file pointer to the offset
		SetFilePointer(hFile, offset, NULL, FILE_BEGIN);

		// Write the data to the file
		DWORD bytesWritten;
		if (WriteFile(hFile, data.Data(), data.Size(), &bytesWritten, NULL) == 0)
		{
			// Log the error
			spdlog::warn("Failed to write to the file: {}", GetLastErrorMessage());

			CloseHandle(hFile);
			return false;
		}

		// Close the file
		CloseHandle(hFile);
		return true;
	}

	u64 PlatformFile::GetSize(const Path& path)
	{
		// Check if the file exists
		if (!Exists(path))
		{
			spdlog::warn("Failed to get the file size: File does not exist");
			return 0;
		}

		// Open the file
		HANDLE hFile = CreateFileA(path.string().c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		{
			spdlog::warn("Failed to get the file size: {}", GetLastErrorMessage());
			return 0;
		}

		// Get the file size
		DWORD fileSize = GetFileSize(hFile, NULL);
		if (fileSize == INVALID_FILE_SIZE)
		{
			spdlog::warn("Failed to get the file size: {}", GetLastErrorMessage());
			CloseHandle(hFile);
			return 0;
		}

		// Close the file
		CloseHandle(hFile);
		return fileSize;
	}
#endif
}