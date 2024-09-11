#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
    enum class ENGINE_API BlendFactor : u8
    {
        BF_Zero,
        BF_One,
        BF_SrcColor,
        BF_OneMinusSrcColor,
        BF_DstColor,
        BF_OneMinusDstColor,
        BF_SrcAlpha,
        BF_OneMinusSrcAlpha,
        BF_DstAlpha,
        BF_OneMinusDstAlpha,
        BF_ConstantColor,
        BF_OneMinusConstantColor,
        BF_ConstantAlpha,
        BF_OneMinusConstantAlpha,
        BF_SrcAlphaSat,
        BF_Src1Color,
        BF_OneMinusSrc1Color,
        BF_Src1Alpha,
        BF_OneMinusSrc1Alpha
    };
}