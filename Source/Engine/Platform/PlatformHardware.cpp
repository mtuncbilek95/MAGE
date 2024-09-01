#include "PlatformHardware.h"

#if defined(MAGE_WINDOWS)
#include "Engine/Win32/Win32Hardware.h"
typedef MAGE::Win32Hardware Dependency;
#endif

namespace MAGE
{
	const String PlatformHardware::GetCPUName()
	{
		return Dependency::GetCPUName();
	}

	const String PlatformHardware::GetGPUName()
	{
		return Dependency::GetGPUName();
	}

	u64 PlatformHardware::GetHostMemorySize()
	{
		return Dependency::GetHostMemorySize();
	}

	u64 PlatformHardware::GetDeviceMemorySize()
	{
		return Dependency::GetDeviceMemorySize();
	}

	u32 PlatformHardware::GetCPUCores()
	{
		return Dependency::GetCPUCores();
	}

	u32 PlatformHardware::GetCPUThreads()
	{
		return Dependency::GetCPUThreads();
	}
}