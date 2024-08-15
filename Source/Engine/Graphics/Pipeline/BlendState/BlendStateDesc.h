#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Core/LogicOperation.h>
#include <Engine/Graphics/Pipeline/BlendState/BlendStateAttachment.h>

namespace MAGE
{
    struct BlendStateDesc
    {
        b8 bLogicOperationEnabled;
        LogicOperation LogicOperation;
        Vector<BlendStateAttachment> Attachments;
    };
}