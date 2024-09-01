#include "PlatformErrorMessage.h"

#if defined(MAGE_WINDOWS)
#include "Engine/Win32/Win32ErrorMessage.h"
typedef MAGE::Win32ErrorMessage Dependency;
#endif

#include <cstdarg>

#pragma warning(push)
#pragma warning(disable: 5082) // WTF?

namespace MAGE
{
	const String PlatformErrorMessage::GetLastKnownError()
	{
		return Dependency::GetLastKnownError();
	}

	void PlatformErrorMessage::ShowAssert(const String& title, const String& message, ...)
	{
        va_list args;
        va_start(args, message.data());

        // Estimate the required buffer size
        Vector<char> buffer(1024);
        int len = vsnprintf(buffer.data(), buffer.size(), message.c_str(), args);

        if (len < 0) 
        {
            va_end(args);
            return;
        }

        if (len >= static_cast<int>(buffer.size())) 
        {
            buffer.resize(len + 1);
            vsnprintf(buffer.data(), buffer.size(), message.c_str(), args);
        }

        String formattedMessage(buffer.data(), len);
		Dependency::ShowAssert(title, formattedMessage);
        va_end(args);
	}
}

#pragma warning(pop)
