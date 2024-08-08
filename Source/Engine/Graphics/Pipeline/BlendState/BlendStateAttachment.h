#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Pipeline/BlendState/BlendFactor.h>
#include <Engine/Graphics/Pipeline/BlendState/BlendOperation.h>
#include <Engine/Graphics/Pipeline/BlendState/BlendColorWriteMask.h>

namespace MAGE
{
    struct BlendStateAttachment
    {
        b8 bEnabled;
        BlendFactor SourceColorFactor;
        BlendFactor DestinationColorFactor;
        BlendOperation ColorOperation;
        BlendFactor SourceAlphaFactor;
        BlendFactor DestinationAlphaFactor;
        BlendOperation AlphaOperation;
        BlendColorWriteMask WriteMask;
    };
}