#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	struct Win32ErrorMessage final
	{
		static const String GetLastKnownError();
		static void ShowAssert(const String& title, const String& message);
	};
}