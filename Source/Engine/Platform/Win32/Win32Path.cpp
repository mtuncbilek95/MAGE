#include "Win32Path.h"

#include <Windows.h>
#include <ShlObj.h>
#pragma comment(lib, "Shell32.lib")

namespace MAGE
{
	String Win32Path::GetExecutableDir()
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		return path;
	}

	String Win32Path::GetProgramFilesDir()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_PROGRAM_FILES, NULL, 0, buffer);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		return path;
	}

	String Win32Path::GetAppDataDir()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, buffer);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		return path;
	}

	String Win32Path::GetTempDir()
	{
		char buffer[MAX_PATH];
		GetTempPathA(MAX_PATH, buffer);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		return path;
	}
}
