#include "Win32ErrorMessage.h"

#include <Windows.h>

namespace MAGE
{
	const String Win32ErrorMessage::GetLastKnownError()
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

	void Win32ErrorMessage::ShowAssert(const String& title, const String& message)
	{
		i32 code = MessageBoxA(nullptr, message.c_str(), title.c_str(), MB_OKCANCEL | MB_ICONERROR);
		if (code == IDOK)
			__debugbreak();
		else
			ExitProcess(1);
	}
}