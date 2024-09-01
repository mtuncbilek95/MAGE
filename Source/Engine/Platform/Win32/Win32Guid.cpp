#include "Win32Guid.h"

#include <Windows.h>
#include "Engine/Core/Guid.h"

namespace MAGE
{
	void Win32Guid::GenerateGuid(Guid& guidOut)
	{
		HRESULT result = CoCreateGuid((GUID*)&guidOut.GetA());
	}
}
