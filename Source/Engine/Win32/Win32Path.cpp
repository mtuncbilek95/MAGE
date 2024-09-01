#include "Win32Path.h"

#include <Windows.h>
#include <ShlObj.h>

namespace MAGE
{
	const String Win32Path::GetExecPath()
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA(nullptr, buffer, MAX_PATH);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		return path;
	}

	const String Win32Path::GetProgramFilesPath()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_PROGRAM_FILES, NULL, 0, buffer);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		return path;
	}

	const String Win32Path::GetAppDataPath()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, buffer);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		return path;
	}

	const String Win32Path::GetTemporaryPath()
	{
		char buffer[MAX_PATH];
		GetTempPathA(MAX_PATH, buffer);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		return path;
	}

	const String Win32Path::GetHomePath()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, buffer);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		return path;
	}
}