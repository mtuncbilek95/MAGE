#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
    class LinuxPath
    {
    public:
        static String GetExecutableDir();
		static String GetProgramFilesDir();
		static String GetAppDataDir();
		static String GetTempDir();
    }
}