#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
    enum class ENGINE_API TextureFormat : u8
    {
        TF_Unknown,
        TF_RGBA32_Float,
        TF_RGBA32_UInt,
        TF_RGBA32_Int,

        TF_RGB32_Float,
        TF_RGB32_UInt,
        TF_RGB32_Int,

        TF_RGBA16_Float,
        TF_RGBA16_UNorm,
        TF_RGBA16_UInt,
        TF_RGBA16_Norm,
        TF_RGBA16_Int,

        TF_RG32_Float,
        TF_RG32_UInt,
        TF_RG32_Int,

        TF_RGBA8_UNorm,
        TF_RGBA8_UInt,
        TF_RGBA8_Norm,
        TF_RGBA8_Int,
        TF_RGBA8_SRGB,

        TF_RG16_Float,
        TF_RG16_UNorm,
        TF_RG16_UInt,
        TF_RG16_Norm,
        TF_RG16_Int,

        TF_D32_Float,
        TF_R32_Float,
        TF_R32_UInt,
        TF_R32_Int,

        TF_D24_UNorm_S8_UInt,
        TF_D32_Float_S8_UInt,

        TF_RG8_UNorm,
        TF_RG8_UInt,
        TF_RG8_Norm,
        TF_RG8_Int,

        TF_R16_Float,
        TF_D16_UNorm,
        TF_R16_UNorm,
        TF_R16_UInt,
        TF_R16_Norm,
        TF_R16_Int,

        TF_R8_UNorm,
        TF_R8_UInt,
        TF_R8_Norm,
        TF_R8_Int,
        TF_A8_UNorm
    };
}