#pragma once

#include <Engine/Core/Core.h>

namespace MAGE {

class LinuxHardware {
public:
    static u16 GetCPUThreadCount();
    static u64 GetRAMSize();
};

} // MAGE