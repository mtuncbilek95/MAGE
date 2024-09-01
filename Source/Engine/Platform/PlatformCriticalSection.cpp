#include "PlatformCriticalSection.h"

#if defined(MAGE_WINDOWS)
#include "Engine/Win32/Win32CriticalSection.h"
typedef MAGE::Win32CriticalSection Dependency;
#endif

namespace MAGE
{
	SharedPtr<PlatformCriticalSection> PlatformCriticalSection::CreateCS()
	{
		return MakeShared<Dependency>();
	}
}
