#pragma once

#include <Engine/Core/Core.h>

namespace MAGE {
class LinuxMessage {
    public:
    static void ShowMessage(const String& title, const String& message, const i32 flags);
};
} // MAGE