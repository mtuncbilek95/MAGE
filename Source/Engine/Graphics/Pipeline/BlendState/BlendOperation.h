#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
    enum class BlendOperation : u8
    {
        Add,
        Subtract,
        Max,
        Min,
        ReverseSubtract
    };
}