#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	struct PlatformPath final
	{
		static Path GetExecPath();
		static Path GetProgramFilesPath();
		static Path GetAppDataPath();
		static Path GetTemporaryPath();
		static Path GetHomePath();
	};
}