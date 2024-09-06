#include "PlatformErrorMessage.h"

#if defined(MAGE_WINDOWS)
#include <Windows.h>
#endif

#include <cstdarg>

namespace MAGE
{
#if defined(MAGE_WINDOWS)
	const String PlatformErrorMessage::GetLastKnownError()
	{
		// Get the error message ID, if any.
		DWORD errorID = GetLastError();
		if (errorID == 0)
			return String();

		LPSTR messageBuffer = nullptr;

		// Format the error message
		u64 size = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&messageBuffer, 0, NULL);

		// Buffer it up
		String message(messageBuffer, size);

		// Free the buffer and return the message
		LocalFree(messageBuffer);
		return message;
	}

	void PlatformErrorMessage::ShowAssert(const String& title, const char* message, ...)
	{
		va_list args;
		va_start(args, message);

		char buffer[4096];
		vsnprintf(buffer, sizeof(buffer), message, args);

		va_end(args);

		i32 code = MessageBoxA(nullptr, buffer, title.c_str(), MB_OKCANCEL | MB_ICONERROR);
		if (code == IDOK)
			__debugbreak();
		else
			ExitProcess(1);
	}
#endif
}