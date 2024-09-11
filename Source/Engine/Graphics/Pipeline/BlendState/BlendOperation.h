#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
    enum class ENGINE_API BlendOperation : u8
    {
        BO_Add,
        BO_Subtract,
        BO_Max,
        BO_Min,
        BO_ReverseSubtract
    };
}