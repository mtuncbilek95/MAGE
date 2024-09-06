#include "PlatformPath.h"

#if defined(MAGE_WINDOWS)
#include <Windows.h>
#include <ShlObj.h>
#endif

namespace MAGE
{
#if defined(MAGE_WINDOWS)
	Path PlatformPath::GetExecPath()
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA(nullptr, buffer, MAX_PATH);
		Path path = buffer;
		std::ranges::replace(path.string(), '\\', '/');
		return path;
	}

	Path PlatformPath::GetProgramFilesPath()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_PROGRAM_FILES, NULL, 0, buffer);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		Path appDataPath = path + "/Mage";
		return appDataPath;
	}

	Path PlatformPath::GetAppDataPath()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, buffer);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		Path appDataPath = path + "/Mage";
		return appDataPath;
	}

	Path PlatformPath::GetTemporaryPath()
	{
		char buffer[MAX_PATH];
		GetTempPathA(MAX_PATH, buffer);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		Path appDataPath = path + "/Mage";
		return appDataPath;
	}

	Path PlatformPath::GetHomePath()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, buffer);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		Path appDataPath = path + "/Mage";
		return appDataPath;
	}
#endif
}