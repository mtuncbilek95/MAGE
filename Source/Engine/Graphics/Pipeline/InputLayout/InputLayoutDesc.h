#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Pipeline/InputLayout/MeshTopology.h>
#include <Engine/Graphics/Pipeline/InputLayout/InputBinding.h>

namespace MAGE
{
    struct InputLayoutDesc
    {
        MeshTopology Topology;
        DArray<InputBinding> Bindings;
    };
}