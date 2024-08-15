#include "Win32Message.h"

#include <Windows.h>

namespace MAGE
{
	void Win32Message::ShowMessage(const String& title, const String& message, const i32 flags)
	{
		MessageBoxA(NULL, message.c_str(), title.c_str(), flags);
	}
}