#pragma once

#include <Engine/Core/Core.h>

namespace MAGE {
    enum class BlendColorWriteMask : u8 {
        Red = 1 << 0,
        Green = 1 << 1,
        Blue = 1 << 2,
        Alpha = 1 << 3,
        All = 1 << 4
    };

    GENERATE_ENUM_FLAG(MAGE::BlendColorWriteMask, u8);
}
