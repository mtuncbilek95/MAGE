#include "PlatformPath.h"

#if defined(DELUSION_WINDOWS)
#include <Windows.h>
#include <ShlObj.h>
#endif // DELUSION_WINDOWS

#include <algorithm>

namespace MAGE
{
#if defined(DELUSION_WINDOWS)
	String PlatformPath::GetProgramFilesDir()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_PROGRAM_FILES, NULL, 0, buffer);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		path += "/Delusion";
		return path;
	}

	String PlatformPath::GetAppDataDir()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, buffer);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		path += "/Delusion";
		return path;
	}

	String PlatformPath::GetTempDir()
	{
		char buffer[MAX_PATH];
		GetTempPathA(MAX_PATH, buffer);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		path += "Delusion";
		return path;
	}

	String PlatformPath::GetHomeDir()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, buffer);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		path += "/Delusion";
		return path;
	}

	String PlatformPath::GetDesktopDir()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_DESKTOP, NULL, 0, buffer);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		return path;
	}

	String PlatformPath::GetDocumentsDir()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, buffer);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		path += "/Delusion";
		return path;
	}

	String PlatformPath::GetDownloadsDir()
	{
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, buffer);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		path += "/Downloads";
		return path;
	}

	String PlatformPath::GetExecutableDir()
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		String path = buffer;
		std::ranges::replace(path, '\\', '/');
		return path;
	}

	String PlatformPath::GetEngineDir()
	{
		return String();
	}
#endif // DELUSION_WINDOWS
}
