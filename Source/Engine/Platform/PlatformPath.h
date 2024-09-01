#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	struct PlatformPath final
	{
		static const String GetExecPath();
		static const String GetProgramFilesPath();
		static const String GetAppDataPath();
		static const String GetTemporaryPath();
		static const String GetHomePath();
	};
}