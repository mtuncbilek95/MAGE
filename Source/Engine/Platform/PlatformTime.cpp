#include "PlatformTime.h"

#if defined(MAGE_WINDOWS)
#include <Engine/Win32/Win32Time.h>
typedef MAGE::Win32Time PlatformDependency;
#endif

namespace MAGE
{
	Time PlatformTime::CurrentDateTime()
	{
		return PlatformDependency::CurrentDateTime();
	}
}
