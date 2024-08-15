#include "PlatformCriticalSection.h"

#if defined(MAGE_WINDOWS)
#include "Win32/Win32CriticalSection.h"
typedef MAGE::Win32CriticalSection PlatformDependency;
#endif

#if defined(MAGE_LINUX)
// #include "Linux/LinuxCriticalSection.h"
// typedef MAGE::LinuxCriticalSection PlatformDependency;
#endif

namespace MAGE
{
	SharedPtr<PlatformCriticalSection> PlatformCriticalSection::CreateCS()
	{
#if defined(MAGE_WINDOWS)
		return MakeShared<Win32CriticalSection>();
#elif defined(MAGE_LINUX)
		return nullptr; // TODO: Implement Linux critical section
#endif
	}
}
