#include "Win32Error.h"

#include <Windows.h>

namespace MAGE
{
	String Win32Error::GetPlatformError()
	{
		// Retrieve the system error message for the last-error code
		DWORD errorCode = GetLastError();
		if (errorCode == 0)
		{
			return String();
		}

		// Get the error message
		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, nullptr);
		String message(messageBuffer, size);

		// Free the buffer
		LocalFree(messageBuffer);

		return message;
	}
}
