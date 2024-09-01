#include "PlatformGuid.h"

#if defined(MAGE_WINDOWS)
#include "Engine/Win32/Win32Guid.h"
typedef MAGE::Win32Guid Dependency;
#endif

namespace MAGE
{
	void PlatformGuid::GenerateGuid(Guid& guidOut)
	{
		Dependency::GenerateGuid(guidOut);
	}
}
