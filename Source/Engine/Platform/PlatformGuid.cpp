#include "PlatformGuid.h"

#if defined(MAGE_WINDOWS)
#include <Windows.h>
#endif

namespace MAGE
{
#if defined(MAGE_WINDOWS)
	void PlatformGuid::GenerateGuid(Guid& guidOut)
	{
		HRESULT result = CoCreateGuid((GUID*)&guidOut.GetA());
	}
#endif
}
