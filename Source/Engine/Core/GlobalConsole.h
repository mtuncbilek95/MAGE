#pragma once

#include "Definitions.h"
#include "StdFix.h"

namespace MAGE
{
	enum class ConsoleType
	{
		Info,
		Verbose,
		Warning,
		Error
	};

	class GlobalConsole final
	{
	public:
		static void Print(const char* message, ...);
		static void Log(ConsoleType type, const char* message, ...);
		static void Assert(b8 condition, const char* title, const char* message, ...);

	public:
		GlobalConsole() = delete;
		~GlobalConsole() = delete;
	};
}