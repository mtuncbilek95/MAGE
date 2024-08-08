#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Texture/SampleCount.h>

namespace MAGE
{
    struct MultisampleDesc
    {
        b8 bSampleShadingEnabled;
        SampleCount Samples;
    };
}