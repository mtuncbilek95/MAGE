#include "PlatformTime.h"

#if defined(MAGE_WINDOWS)
#include "Win32/Win32Time.h"
typedef MAGE::Win32Time PlatformDependency;
#endif

#if defined(MAGE_LINUX)
#include "Linux/LinuxTime.h"
typedef MAGE::LinuxTime PlatformDependency;
#endif

namespace MAGE
{
	Time PlatformTime::CurrentDateTime()
	{
		return PlatformDependency::CurrentDateTime();
	}
}
