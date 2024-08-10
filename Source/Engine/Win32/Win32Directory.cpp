#include "Win32Directory.h"

#if defined(MAGE_WINDOWS)
#include <Windows.h>
#include <stdio.h>
#endif

namespace MAGE
{
	b8 Win32Directory::Exists(const String& path)
	{
#if defined(MAGE_WINDOWS)
		DWORD dwAttrib = GetFileAttributesA(path.c_str());

		// If the file is found return true
		return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#else
		return false;
#endif
	}

	b8 Win32Directory::Create(const String& path)
	{
#if defined(MAGE_WINDOWS)
		// Create the directory
		return CreateDirectoryA(path.c_str(), NULL) != 0;
#else
		return false;
#endif
	}

	b8 Win32Directory::Delete(const String& path)
	{
#if defined(MAGE_WINDOWS)
		// Delete the directory
		return RemoveDirectoryA(path.c_str()) != 0;
#else
		return false;
#endif
	}

	b8 Win32Directory::Copy(const String& source, const String& destination)
	{
#if defined(MAGE_WINDOWS)
		// Copy the directory
		return CopyFileA(source.c_str(), destination.c_str(), TRUE) != 0;
#else
		return false;
#endif
	}

	b8 Win32Directory::Move(const String& source, const String& destination)
	{
#if defined(MAGE_WINDOWS)
		// Move the directory
		return MoveFileA(source.c_str(), destination.c_str()) != 0;
#else
		return false;
#endif
	}

	b8 Win32Directory::Rename(const String& source, const String& destination)
	{
#if defined(MAGE_WINDOWS)
		// Rename the directory
		return MoveFileA(source.c_str(), destination.c_str()) != 0;
#else
		return false;
#endif
	}

	b8 Win32Directory::GetFiles(const String& path, DArray<String>& files)
	{
#if defined(MAGE_WINDOWS)
		// Check if the directory exists
		if (!Exists(path))
			return false;

		WIN32_FIND_DATAA findFileData;
		HANDLE hFind;

		// Find the first file in the directory
		hFind = FindFirstFileA((path + "/*").c_str(), &findFileData);

		// Check if the file was found
		if (hFind == INVALID_HANDLE_VALUE)
			return false;

		// Loop through all the files in the directory
		do
		{
			// Check if the file is a directory
			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				files.push_back(findFileData.cFileName);
			}
		} while (FindNextFileA(hFind, &findFileData) != 0);

		// Close the handle
		FindClose(hFind);
		return true;
#else
		return false;
#endif
	}

	b8 Win32Directory::GetDirectories(const String& path, DArray<String>& directories)
	{
#if defined(MAGE_WINDOWS)
		// Check if the directory exists
		if (!Exists(path))
			return false;

		WIN32_FIND_DATAA findFileData;
		HANDLE hFind;

		// Find the first file in the directory
		hFind = FindFirstFileA((path + "/*").c_str(), &findFileData);

		// Check if the file was found
		if (hFind == INVALID_HANDLE_VALUE)
			return false;

		// Loop through all the files in the directory
		do
		{
			// Check if the file is a directory
			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				directories.push_back(findFileData.cFileName);
			}
		} while (FindNextFileA(hFind, &findFileData) != 0);

		// Close the handle
		FindClose(hFind);
		return true;
#else
		return false;
#endif

	}

	b8 Win32Directory::GetFilesWithExtension(const String& path, const String& extension, DArray<String>& files)
	{
#if defined(MAGE_WINDOWS)
		// Check if the directory exists
		if (!Exists(path))
			return false;

		WIN32_FIND_DATAA findFileData;
		HANDLE hFind;

		// Find the first file in the directory
		hFind = FindFirstFileA((path + "/*." + extension).c_str(), &findFileData);

		// Check if the file was found
		if (hFind == INVALID_HANDLE_VALUE)
			return false;

		// Loop through all the files in the directory
		do
		{
			// Check if the file is a directory
			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				files.push_back(findFileData.cFileName);
			}
		} while (FindNextFileA(hFind, &findFileData) != 0);

		// Close the handle
		FindClose(hFind);
		return true;
#else
		return false;
#endif
	}

}
