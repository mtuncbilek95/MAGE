#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Pipeline/DepthStencilState/StencilOperation.h>
#include <Engine/Graphics/Core/CompareOperation.h>

namespace MAGE
{
    struct StencilFaceDesc
    {
        StencilOperation FailOperation;
        StencilOperation PassOperation;
        StencilOperation DepthFailOperation;
        CompareOperation StencilCompareOperation;
        u32 CompareMask;
        u32 WriteMask;
        u32 Reference;
    };
}