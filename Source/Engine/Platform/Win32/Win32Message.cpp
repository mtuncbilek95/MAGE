#include "Win32Message.h"

#if defined(MAGE_WINDOWS)
#include <Windows.h>
#endif

namespace MAGE
{
	void Win32Message::ShowMessage(const String& title, const String& message, const i32 flags)
	{
#if defined(MAGE_WINDOWS)
		MessageBoxA(NULL, message.c_str(), title.c_str(), flags);
#else
		return;
#endif
	}
}
