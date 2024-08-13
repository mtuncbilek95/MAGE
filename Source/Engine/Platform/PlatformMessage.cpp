#include "PlatformMessage.h"

#if defined(MAGE_WINDOWS)
#include "Win32/Win32Message.h"
typedef MAGE::Win32Message PlatformDependency;
#endif

namespace MAGE
{
	void PlatformMessage::ShowMessage(const String& title, const String& message, const i32 flags)
	{
		PlatformDependency::ShowMessage(title, message, flags);
	}
}
