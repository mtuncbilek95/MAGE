#include "Win32Directory.h"

#include <Windows.h>
#include <spdlog/spdlog.h>

namespace MAGE
{
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

	b8 Win32Directory::Exists(const String& path)
	{
		DWORD dwAttrib = GetFileAttributesA(path.c_str());

		// If the file is found return true
		return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	b8 Win32Directory::Create(const String& path)
	{
		if (CreateDirectoryA(path.c_str(), NULL) == 0)
		{
			// Log the error
			spdlog::warn("Failed to create the directory: {}", GetLastErrorMessage());
			return false;
		}

		return true;
	}

	b8 Win32Directory::Delete(const String& path)
	{
		if (RemoveDirectoryA(path.c_str()) == 0)
		{
			// Log the error
			spdlog::warn("Failed to delete the directory: {}", GetLastErrorMessage());
			return false;
		}

		return true;
	}

	b8 Win32Directory::Copy(const String& source, const String& destination)
	{
		return b8();
	}

	b8 Win32Directory::Move(const String& source, const String& destination)
	{
		return b8();
	}

	b8 Win32Directory::Rename(const String& source, const String& destination)
	{
		return b8();
	}

	Vector<String> Win32Directory::GetFiles(const String& path)
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
		hFind = FindFirstFileA((path + "/*").c_str(), &findFileData);

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

	Vector<String> Win32Directory::GetDirectories(const String& path)
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
		hFind = FindFirstFileA((path + "/*").c_str(), &findFileData);

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

	Vector<String> Win32Directory::GetFilesByExtension(const String& path, const String& extension)
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
		hFind = FindFirstFileA((path + "/*." + extension).c_str(), &findFileData);

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
}