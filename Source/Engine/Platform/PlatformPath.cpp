#include "PlatformPath.h"

#if defined(DELUSION_WINDOWS)
#include <Windows.h>
#include <ShlObj.h>
#endif // DELUSION_WINDOWS

#include <algorithm>

namespace MAGE
{
#if defined(DELUSION_WINDOWS)
	string PlatformPath::GetProgramFilesDir()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_PROGRAM_FILES, NULL, 0, buffer);
		string path = buffer;
		std::ranges::replace(path, '\\', '/');
		path += "/Delusion";
		return path;
	}

	string PlatformPath::GetAppDataDir()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, buffer);
		string path = buffer;
		std::ranges::replace(path, '\\', '/');
		path += "/Delusion";
		return path;
	}

	string PlatformPath::GetTempDir()
	{
		char buffer[MAX_PATH];
		GetTempPathA(MAX_PATH, buffer);
		string path = buffer;
		std::ranges::replace(path, '\\', '/');
		path += "Delusion";
		return path;
	}

	string PlatformPath::GetHomeDir()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, buffer);
		string path = buffer;
		std::ranges::replace(path, '\\', '/');
		path += "/Delusion";
		return path;
	}

	string PlatformPath::GetDesktopDir()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_DESKTOP, NULL, 0, buffer);
		string path = buffer;
		std::ranges::replace(path, '\\', '/');
		return path;
	}

	string PlatformPath::GetDocumentsDir()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, buffer);
		string path = buffer;
		std::ranges::replace(path, '\\', '/');
		path += "/Delusion";
		return path;
	}

	string PlatformPath::GetDownloadsDir()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, buffer);
		string path = buffer;
		std::ranges::replace(path, '\\', '/');
		path += "/Downloads";
		return path;
	}

	string PlatformPath::GetExecutableDir()
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		string path = buffer;
		std::ranges::replace(path, '\\', '/');
		return path;
	}
#endif // DELUSION_WINDOWS
}
