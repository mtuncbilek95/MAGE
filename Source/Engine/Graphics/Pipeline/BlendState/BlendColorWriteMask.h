#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
    enum class BlendColorWriteMask : u8
    {
        Red = 1,
        Green = 2,
        Blue = 4,
        Alpha = 8,
        All = Red | Green | Blue | Alpha
    };

    GENERATE_ENUM_FLAG(BlendColorWriteMask, u8);
}