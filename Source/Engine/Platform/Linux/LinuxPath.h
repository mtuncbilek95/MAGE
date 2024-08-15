#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
#if defined(MAGE_LINUX)
    class LinuxPath
    {
    public:
        static String GetExecutableDir();
        static String GetProgramFilesDir();
        static String GetAppDataDir();
        static String GetTempDir();
    };
#endif
}