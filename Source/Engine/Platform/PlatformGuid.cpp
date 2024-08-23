#include "PlatformGuid.h"

#if defined(MAGE_WINDOWS)
#include "Win32/Win32Guid.h"
typedef MAGE::Win32Guid PlatformDependency;
#endif

#if defined(MAGE_LINUX)
#include <Engine/Platform/Linux/LinuxGuid.h>
typedef MAGE::LinuxGuid PlatformDependency;
#endif

namespace MAGE
{
	void PlatformGuid::GenerateGuid(Guid& guidOut)
	{
		PlatformDependency::GenerateGuid(guidOut);
	}
}
