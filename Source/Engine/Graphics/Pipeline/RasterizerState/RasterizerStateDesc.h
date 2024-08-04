#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Pipeline/RasterizerState/PolygonMode.h>
#include <Engine/Graphics/Pipeline/RasterizerState/FaceCullMode.h>

namespace MAGE
{
    struct RasterizerStateDesc
    {
        bool bDepthBiasEnabled;
        f32 DepthBiasClamp;
        f32 DepthBiasFactor;
        f32 DepthBiasSlope;
        PolygonMode FillMode;
        FaceCullMode CullFlags;
        bool bFrontCounterClockwise;
    };
}