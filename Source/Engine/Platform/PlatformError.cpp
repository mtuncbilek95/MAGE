#include "PlatformError.h"

#if defined(MAGE_WINDOWS)
#include <Engine/Win32/Win32Error.h>
typedef MAGE::Win32Error PlatformDependency;
#endif

namespace MAGE
{
	String PlatformError::GetPlatformError()
	{
		return PlatformDependency::GetPlatformError();
	}
}
