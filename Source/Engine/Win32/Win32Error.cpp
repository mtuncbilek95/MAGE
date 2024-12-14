#include "Win32Error.h"

#if defined(MAGE_WINDOWS)
#include <Windows.h>
#endif

namespace Mage
{
	string Win32Error::GetLastMessage()
	{
		// Get error code from the system
		DWORD errorID = GetLastError();
		if (errorID == 0)
			return std::string();

		LPSTR messageBuffer = nullptr;

		// Get the message from the system
		u64 size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, errorID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, nullptr);

		// Buffer it up
		std::string message(messageBuffer, size);

		// Free the buffer
		LocalFree(messageBuffer);
		return message;
	}
}