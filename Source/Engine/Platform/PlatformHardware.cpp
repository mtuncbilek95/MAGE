#include "PlatformHardware.h"

#if defined(MAGE_WINDOWS)
#include "Win32/Win32Hardware.h"
typedef MAGE::Win32Hardware PlatformDependency;
#endif

#if defined(MAGE_LINUX)
#include "Linux/LinuxHardware.h"
typedef MAGE::LinuxHardware PlatformDependency;
#endif

namespace MAGE
{
	u16 PlatformHardware::GetCPUThreadCount()
	{
		return PlatformDependency::GetCPUThreadCount();
	}

	u64 PlatformHardware::GetRAMSize()
	{
		return PlatformDependency::GetRAMSize();
	}
}
