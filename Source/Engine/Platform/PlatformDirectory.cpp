#include "PlatformDirectory.h"

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

	b8 PlatformDirectory::Exists(const Path& path)
	{
		DWORD dwAttrib = GetFileAttributesA(path.string().c_str());

		// If the file is found return true
		return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	b8 PlatformDirectory::Create(const Path& path)
	{
		if (CreateDirectoryA(path.string().c_str(), NULL) == 0)
		{
			// Log the error
			spdlog::warn("Failed to create the directory: {}", GetLastErrorMessage());
			return false;
		}

		return true;
	}

	b8 PlatformDirectory::Delete(const Path& path)
	{
		if (RemoveDirectoryA(path.string().c_str()) == 0)
		{
			// Log the error
			spdlog::warn("Failed to delete the directory: {}", GetLastErrorMessage());
			return false;
		}

		return true;
	}

	b8 PlatformDirectory::Copy(const Path& source, const Path& destination)
	{
		return b8();
	}

	b8 PlatformDirectory::Move(const Path& source, const Path& destination)
	{
		return b8();
	}

	b8 PlatformDirectory::Rename(const Path& source, const Path& destination)
	{
		return b8();
	}

	Vector<String> PlatformDirectory::GetFiles(const Path& path)
	{
		// Check if the directory exists
		if (!Exists(path))
		{
			spdlog::warn("Failed to get files: Directory does not exist");
			return Vector<String>();
		}

		WIN32_FIND_DATAA findFileData;
		HANDLE hFind;

		// Find the first file in the directory
		hFind = FindFirstFileA((path.string() + "/*").c_str(), &findFileData);

		// Check if the file was found
		if (hFind == INVALID_HANDLE_VALUE)
		{
			spdlog::warn("Failed to get files: {}", GetLastErrorMessage());
			return Vector<String>();
		}

		Vector<String> files;

		// Loop through all the files in the directory
		while (FindNextFileA(hFind, &findFileData) != 0)
		{
			// Check if the file is a directory
			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				files.push_back(findFileData.cFileName);
		}

		// Close the handle
		FindClose(hFind);
		return files;
	}

	Vector<String> PlatformDirectory::GetDirectories(const Path& path)
	{
		// Check if the directory exists
		if (!Exists(path))
		{
			spdlog::warn("Failed to get directories: Directory does not exist");
			return Vector<String>();
		}

		WIN32_FIND_DATAA findFileData;
		HANDLE hFind;

		// Find the first file in the directory
		hFind = FindFirstFileA((path.string() + "/*").c_str(), &findFileData);

		// Check if the file was found
		if (hFind == INVALID_HANDLE_VALUE)
		{
			spdlog::warn("Failed to get directories: {}", GetLastErrorMessage());
			return Vector<String>();
		}

		Vector<String> directories;

		// Loop through all the files in the directory
		while (FindNextFileA(hFind, &findFileData) != 0)
		{
			// Check if the file is a directory
			if ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
				strcmp(findFileData.cFileName, ".") != 0 &&
				strcmp(findFileData.cFileName, "..") != 0)
				directories.push_back(findFileData.cFileName);
		}

		// Close the handle
		FindClose(hFind);
		return directories;
	}

	Vector<String> PlatformDirectory::GetFilesByExtension(const Path& path, const String& extension)
	{
		// Check if the directory exists
		if (!Exists(path))
		{
			spdlog::warn("Failed to get files by extension: Directory does not exist");
			return Vector<String>();
		}

		WIN32_FIND_DATAA findFileData;
		HANDLE hFind;

		// Find the first file in the directory
		hFind = FindFirstFileA((path.string() + "/*." + extension).c_str(), &findFileData);

		// Check if the file was found
		if (hFind == INVALID_HANDLE_VALUE)
		{
			spdlog::warn("Failed to get files by extension: {}", GetLastErrorMessage());
			return Vector<String>();
		}

		Vector<String> files;

		// Loop through all the files in the directory
		do
		{
			// Check if the file is a directory
			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				files.push_back(findFileData.cFileName);

		} while (FindNextFileA(hFind, &findFileData) != 0);

		// Close the handle
		FindClose(hFind);
		return files;
	}
#endif
}