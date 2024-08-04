#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Texture/SampleCount.h>

namespace MAGE
{
    struct MultisampleDesc
    {
        bool bSampleShadingEnabled;
        SampleCount Samples;
    };
}