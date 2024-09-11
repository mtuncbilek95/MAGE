#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
    enum class ENGINE_API StencilOperation : u8
    {
        SO_Keep,
        SO_Zero,
        SO_Replace,
        SO_IncrementAndClamp,
        SO_DecrementAndClamp,
        SO_Invert,
        SO_IncrementAndWrap,
        SO_DecrementAndWrap
    };
}