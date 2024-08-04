#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Core/CompareOperation.h>
#include <Engine/Graphics/Pipeline/DepthStencilState/StencilFaceDesc.h>

namespace MAGE
{
    struct DepthStencilStateDesc
    {
        bool bDepthTestEnabled;
        bool bDepthWriteEnabled;
        bool bStencilTestEnabled;
        CompareOperation DepthTestOperation;
        StencilFaceDesc StencilFrontFace;
        StencilFaceDesc StencilBackFace;
    };
}