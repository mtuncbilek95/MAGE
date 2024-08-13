#include "PlatformPath.h"

#if defined(MAGE_WINDOWS)
#include "Win32/Win32Path.h"
typedef MAGE::Win32Path PlatformDependency;
#endif

namespace MAGE
{
	String PlatformPath::GetExecutableDir()
	{
		return PlatformDependency::GetExecutableDir();
	}

	String PlatformPath::GetProgramFilesDir()
	{
		return PlatformDependency::GetProgramFilesDir();
	}

	String PlatformPath::GetAppDataDir()
	{
		return PlatformDependency::GetAppDataDir();
	}

	String PlatformPath::GetTempDir()
	{
		return PlatformDependency::GetTempDir();
	}
}	
