#include "PlatformPath.h"

#if defined(MAGE_WINDOWS)
#include "Engine/Win32/Win32Path.h"
typedef MAGE::Win32Path Dependency;
#endif

namespace MAGE
{
	const String PlatformPath::GetExecPath() { return Dependency::GetExecPath(); }
	const String PlatformPath::GetProgramFilesPath() { return Dependency::GetProgramFilesPath(); }
	const String PlatformPath::GetAppDataPath() { return Dependency::GetAppDataPath(); }
	const String PlatformPath::GetTemporaryPath() { return Dependency::GetTemporaryPath(); }
	const String PlatformPath::GetHomePath() { return Dependency::GetHomePath(); }
}