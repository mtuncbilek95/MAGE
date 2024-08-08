#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Core/CompareOperation.h>
#include <Engine/Graphics/Pipeline/DepthStencilState/StencilFaceDesc.h>

namespace MAGE
{
    struct DepthStencilStateDesc
    {
        b8 bDepthTestEnabled;
        b8 bDepthWriteEnabled;
        b8 bStencilTestEnabled;
        CompareOperation DepthTestOperation;
        StencilFaceDesc StencilFrontFace;
        StencilFaceDesc StencilBackFace;
    };
}