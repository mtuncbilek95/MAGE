#include "LinuxHardware.h"

#include <unistd.h>

namespace MAGE {
    u16 LinuxHardware::GetCPUThreadCount() {

        const i64 nThreads = sysconf(_SC_NPROCESSORS_ONLN);

        if (nThreads == -1)
            return 0;

        if (nThreads > u16_max) {
            return u16_max;
        }

        return static_cast<u16>(nThreads);
    }

    u64 LinuxHardware::GetRAMSize() {
        FILE *meminfo = fopen("/proc/meminfo", "r");
        if(meminfo == nullptr)
            return 0;

        char line[256];
        while(fgets(line, sizeof(line), meminfo))
        {
            int ram;
            if(sscanf(line, "MemTotal: %d kB", &ram) == 1)
            {
                fclose(meminfo);
                return ram;
            }
        }

        fclose(meminfo);
        return 0;
    }
}
