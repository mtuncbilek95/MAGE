#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Descriptor/DescriptorLayout.h>

namespace MAGE
{
    struct ResourceLayoutDesc
    {
        DArray<DescriptorLayout*> ResourceLayouts;
    };
}