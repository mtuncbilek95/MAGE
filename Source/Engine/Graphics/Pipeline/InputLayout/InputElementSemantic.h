#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
    enum class InputElementSemantic : u8
    {
        IES_Position,
        IES_Normal,
        IES_Tangent,
        IES_BiTangent,
        IES_Color,
        IES_TexCoord
    };
}