#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
    enum class ENGINE_API BlendColorWriteMask : u8
    {
        CWM_Red = 1 << 0,
        CWM_Green = 1 << 1,
        CWM_Blue = 1 << 2,
        CWM_Alpha = 1 << 3,
        CWM_All = 1 << 4
    };
}