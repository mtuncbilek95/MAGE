#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Time/Time.h>

namespace MAGE {
class LinuxTime {
public:
    static Time CurrentDateTime();
};
} // MAGE