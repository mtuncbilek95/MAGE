#include "Win32Hardware.h"

#if defined(MAGE_WINDOWS)
#include <Windows.h>
#endif

namespace MAGE
{
	u16 Win32Hardware::GetCPUThreadCount()
	{
		SYSTEM_INFO sysInfo;
		GetSystemInfo(&sysInfo);
		return sysInfo.dwNumberOfProcessors;
	}

	u64 Win32Hardware::GetRAMSize()
	{
		MEMORYSTATUSEX memStatus;
		memStatus.dwLength = sizeof(memStatus);
		GlobalMemoryStatusEx(&memStatus);
		return memStatus.ullTotalPhys;
	}
}