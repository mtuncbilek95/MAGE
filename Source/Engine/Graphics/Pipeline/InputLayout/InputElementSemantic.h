#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
    enum class InputElementSemantic : u8
    {
        Position,
        Normal,
        Tangent,
        BiTangent,
        Color,
        TexCoord
    };
}