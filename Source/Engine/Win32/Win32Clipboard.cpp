#include "Win32Clipboard.h"

#if defined(MAGE_WINDOWS)
#include <Windows.h>
#endif

namespace MAGE
{
	void Win32Clipboard::SetClipboardText(const String& text)
	{
#if defined(MAGE_WINDOWS)
		OpenClipboard(nullptr);
		EmptyClipboard();

		const HGLOBAL globalMemory = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
		if (globalMemory)
		{
			memcpy_s(GlobalLock(globalMemory), text.size() + 1, text.c_str(), text.size() + 1);
			GlobalUnlock(globalMemory);
			SetClipboardData(CF_TEXT, globalMemory);
		}

		CloseClipboard();
		GlobalFree(globalMemory);
#endif
	}

	String Win32Clipboard::GetClipboardText()
	{
#if defined(MAGE_WINDOWS)
		OpenClipboard(nullptr);

		const HANDLE clipboardData = GetClipboardData(CF_TEXT);

		if (clipboardData)
		{
			GlobalLock(clipboardData);
			String text = static_cast<char*>(GlobalLock(clipboardData));
			GlobalUnlock(clipboardData);
			 
			CloseClipboard();

			return text;
		}

		CloseClipboard();
		return String();
#else
		return String();
#endif
	}


}
