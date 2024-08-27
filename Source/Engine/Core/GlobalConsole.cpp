#include "GlobalConsole.h"

#include <cstdarg>

#include <Engine/Platform/PlatformMessage.h>
#include <Engine/Platform/PlatformTime.h>

namespace MAGE
{
	void GlobalConsole::Print(const char* message, ...)
	{
		va_list args;
		va_start(args, message);
		vprintf(message, args);
		va_end(args);
	}

	void GlobalConsole::Log(ConsoleType type, const char* message, ...)
	{
		va_list args;
		va_start(args, message);

		const auto [Day, Month, Year, Hour, Minute, Second, Millisecond] = PlatformTime::CurrentDateTime();
		switch (type)
		{
		case ConsoleType::CT_Info:
			printf("[%02d.%02d.%02d][%02d:%02d:%02d][\033[36mDebug\033[39m] -- ", Day, Month, Year, Hour, Minute, Second);
			break;
		case ConsoleType::CT_Verbose:
			printf("[%02d.%02d.%02d][%02d:%02d:%02d][\033[32mTrace\033[39m] -- ", Day, Month, Year, Hour, Minute, Second);
			break;
		case ConsoleType::CT_Warning:
			printf("[%02d.%02d.%02d][%02d:%02d:%02d][\033[33mWarning\033[39m] -- ", Day, Month, Year, Hour, Minute, Second);
			break;
		case ConsoleType::CT_Error:
			printf("[%02d.%02d.%02d][%02d:%02d:%02d][\033[31mError\033[39m] -- ", Day, Month, Year, Hour, Minute, Second);
			break;
		}

		vprintf(message, args);
		printf("\n");
		va_end(args);
	}

	void GlobalConsole::Assert(b8 condition, const char* title, const char* message, ...)
	{
		if (!condition)
		{
			va_list args;
			va_start(args, message);
			printf("\033[31m[%s]\033[39m -- ", title);
			vprintf(message, args);

			String msg; 

			// Add args to msg
			while (*message != '\0')
			{
				if (*message == '%')
				{
					message++;
					if (*message == 'd')
					{
						int i = va_arg(args, int);
						msg += std::to_string(i);
					}
					else if (*message == 'f')
					{
						double d = va_arg(args, double);
						msg += std::to_string(d);
					}
					else if (*message == 's')
					{
						char* s = va_arg(args, char*);
						msg += s;
					}
				}
				else
				{
					msg += *message;
				}
				message++;
			}

			PlatformMessage::ShowMessage("Assertion Failed", msg, Mb_Ok | Mb_IconError);

			printf("\n");
			va_end(args);

			exit(-1);
		}
	}
}
