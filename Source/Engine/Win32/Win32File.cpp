#include "Win32File.h"

#if defined(MAGE_WINDOWS)
#include <Windows.h>
#endif

#include "Engine/Win32/Win32Error.h"
#include <spdlog/spdlog.h>

namespace Mage
{
	b8 Win32File::Exists(const path& dst)
	{
		// Check if the file exists
		DWORD attribute = GetFileAttributesA(dst.string().c_str());
		return (attribute != INVALID_FILE_ATTRIBUTES && !(attribute & FILE_ATTRIBUTE_DIRECTORY));
	}

	b8 Win32File::Create(const path& dst)
	{
		// Create the file
		HANDLE hFile = CreateFileA(dst.string().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		// Check if the file was created
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		{
			// Log the error
			spdlog::warn("Failed to create the file: {}", Win32Error::GetLastMessage());
			return false;
		}

		// Close the file
		CloseHandle(hFile);
		return true;
	}

	b8 Win32File::Read(const path& dst, ownBuf& buffer, b8 null, usize start, usize end)
	{
		// Check if the file exists
		if (!Win32File::Exists(dst))
		{
			spdlog::warn("Failed to read the file: File does not exist");
			return false;
		}

		// Open the file
		HANDLE hFile = CreateFileA(dst.string().c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		{
			spdlog::warn("Failed to read the file: {}", Win32Error::GetLastMessage());
			return false;
		}

		// Get the file size
		LARGE_INTEGER fileSize;
		if (!GetFileSizeEx(hFile, &fileSize))
		{
			spdlog::warn("Failed to read the file: {}", Win32Error::GetLastMessage());
			CloseHandle(hFile);
			return false;
		}

		// Read the file
		DWORD bytesRead;
		char* data = new char[fileSize.QuadPart + (null ? 1 : 0)];
		if (ReadFile(hFile, data, fileSize.QuadPart, &bytesRead, NULL) == 0)
		{
			spdlog::warn("Failed to read the file: {}", Win32Error::GetLastMessage());
			CloseHandle(hFile);
			delete[] data;
			return false;
		}

		// Close the file
		CloseHandle(hFile);

		// Null terminate the buffer
		if (null)
			data[fileSize.QuadPart] = '\0';

		// Set the output
		buffer = OwnedMemory(data, fileSize.QuadPart);
		delete[] data;

		return true;
	}

	b8 Win32File::Read(const path& dst, string& buffer, usize start, usize end)
	{
		// Check if the file exists
		if (!Win32File::Exists(dst))
		{
			spdlog::warn("Failed to read the file: File does not exist");
			return false;
		}

		// Open the file
		HANDLE hFile = CreateFileA(dst.string().c_str(), GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		{
			spdlog::warn("Failed to read the file: {}", Win32Error::GetLastMessage());
			return false;
		}

		// Get the file size
		LARGE_INTEGER fileSize;
		if (!GetFileSizeEx(hFile, &fileSize))
		{
			spdlog::warn("Failed to read the file: {}", Win32Error::GetLastMessage());
			CloseHandle(hFile);
			return false;
		}

		// Read the file
		DWORD bytesRead;
		buffer.resize(fileSize.QuadPart + 1);
		if (ReadFile(hFile, &buffer[0], fileSize.QuadPart, &bytesRead, NULL) == 0)
		{
			spdlog::warn("Failed to read the file: {}", Win32Error::GetLastMessage());
			CloseHandle(hFile);
			return false;
		}

		// Close the file
		CloseHandle(hFile);
		buffer[fileSize.QuadPart] = '\0';

		return true;
	}

	b8 Win32File::Write(const path& dst, const shareBuf& buffer, usize start)
	{
		HANDLE hFile;

		// Check if the file exists
		if (Win32File::Exists(dst))
			hFile = CreateFileA(dst.string().c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		else
			hFile = CreateFileA(dst.string().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		// Check if the file was created
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
			return false;

		// Set the file pointer to the offset
		SetFilePointer(hFile, start, NULL, FILE_BEGIN);

		// Write the data to the file
		DWORD bytesWritten;
		if (WriteFile(hFile, buffer.Data(), buffer.Size(), &bytesWritten, NULL) == 0)
		{
			// Log the error
			spdlog::warn("Failed to write to the file: {}", Win32Error::GetLastMessage());

			CloseHandle(hFile);
			return false;
		}

		// Close the file
		CloseHandle(hFile);
		return true;
	}

	b8 Win32File::Write(const path& dst, const string& buffer, usize start)
	{
		HANDLE hFile;

		// Check if the file exists
		if (Win32File::Exists(dst))
			hFile = CreateFileA(dst.string().c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		else
			hFile = CreateFileA(dst.string().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		// Check if the file was created
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
			return false;

		// Set the file pointer to the offset
		SetFilePointer(hFile, start, NULL, FILE_BEGIN);

		// Write the data to the file
		DWORD bytesWritten;
		if (WriteFile(hFile, buffer.data(), buffer.size(), &bytesWritten, NULL) == 0)
		{
			// Log the error
			spdlog::warn("Failed to write to the file: {}", Win32Error::GetLastMessage());

			CloseHandle(hFile);
			return false;
		}

		// Close the file
		CloseHandle(hFile);
		return true;
	}

	b8 Win32File::Rename(const path& src, const path& dst)
	{
		return b8();
	}

	b8 Win32File::Copy(const path& src, const path& dst)
	{
		return b8();
	}

	b8 Win32File::Move(const path& src, const path& dst)
	{
		return b8();
	}

	b8 Win32File::Delete(const path& dst)
	{
		if (DeleteFileA(dst.string().c_str()) == 0)
		{
			spdlog::warn("Failed to delete the file: {}", Win32Error::GetLastMessage());
			return false;
		}
		return true;
	}
}