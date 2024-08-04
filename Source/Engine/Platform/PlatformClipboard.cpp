#include "PlatformClipboard.h"

#if defined(MAGE_WINDOWS)
#include <Engine/Win32/Win32Clipboard.h>
typedef MAGE::Win32Clipboard PlatformDependency;
#endif

namespace MAGE
{
	void PlatformClipboard::SetClipboardText(const String& text)
	{
		PlatformDependency::SetClipboardText(text);
	}

	String PlatformClipboard::GetClipboardText()
	{
		return PlatformDependency::GetClipboardText();
	}
}
