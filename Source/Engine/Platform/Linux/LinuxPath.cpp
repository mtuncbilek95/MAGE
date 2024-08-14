#include "LinuxPath.h"

#include <filesystem>

namespace MAGE
{
    String LinuxPath::GetExecutableDir()
    {
        char buffer[1024];
        s64 len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);

        CORE_ASSERT(len != -1, "LinuxPath", "Failed to get executable directory");

        buffer[len] = '\0';
        return String(buffer);
    }

    String LinuxPath::GetProgramFilesDir()
    {
        return "/usr/bin";
    }

    String LinuxPath::GetAppDataDir()
    {
        return "/usr/share";
    }

    String LinuxPath::GetTempDir()
    {
        return "/tmp";
    }
}