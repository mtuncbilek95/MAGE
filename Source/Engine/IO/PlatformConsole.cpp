#include "PlatformConsole.h"

#if defined(DELUSION_WINDOWS)
#include <Windows.h>
#endif // DELUSION_WINDOWS

#include "Engine/ErrorHandler/ErrorChecker.h"

namespace MAGE
{
#if defined(DELUSION_WINDOWS)
	void PlatformConsole::ShowConsole()
	{
		HWND consoleWindow = GetConsoleWindow();
		ShowWindow(consoleWindow, SW_SHOW);
	}

	void PlatformConsole::HideConsole()
	{
		HWND consoleWindow = GetConsoleWindow();
		ShowWindow(consoleWindow, SW_HIDE);
	}

	void PlatformConsole::BlowConsole()
	{
		HWND consoleWindow = GetConsoleWindow();
		PostMessage(consoleWindow, WM_QUIT, 0, 0);
	}
}
#endif // DELUSION_WINDOWS
