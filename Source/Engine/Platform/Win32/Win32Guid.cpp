#include "Win32Guid.h"

#if defined(MAGE_WINDOWS)
#include <Windows.h>
#include <Engine/Core/Guid.h>
#endif

namespace MAGE
{
	void Win32Guid::GenerateGuid(Guid& guidOut)
	{
#if defined(MAGE_WINDOWS)
		HRESULT result = CoCreateGuid((GUID*)&guidOut.GetA());
#else
		return;
#endif
	}
}
