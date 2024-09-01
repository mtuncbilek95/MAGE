#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
    enum class BlendOperation : u8
    {
        BO_Add,
        BO_Subtract,
        BO_Max,
        BO_Min,
        BO_ReverseSubtract
    };
}