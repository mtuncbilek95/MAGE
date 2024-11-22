#include "ErrorChecker.h"

#if defined(DELUSION_WINDOWS)
#include <Windows.h>
#endif // DELUSION_WINDOWS

namespace MAGE
{
	namespace ErrorUtils
	{
#if defined(DELUSION_WINDOWS)
		string Handler::GetErrorString()
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

		void Handler::PlatformMessage(const string& title, const char* message, ...)
		{
			// Format the message
			va_list args;
			va_start(args, message);

			// Buffer it up
			char buffer[4096];
			vsnprintf(buffer, sizeof(buffer), message, args);
			va_end(args);

			// Show the message box
			i32 code = MessageBoxA(nullptr, buffer, title.c_str(), MB_OKCANCEL | MB_ICONERROR);

#if defined(DELUSION_DEBUG)
			if (code == IDOK)
				__debugbreak();
			else
				ExitProcess(-99);
#else
			ExitProcess(-99);
#endif
		}
	}
#endif
}
