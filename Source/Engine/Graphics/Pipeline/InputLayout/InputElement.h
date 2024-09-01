#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Texture/TextureFormat.h"
#include "Engine/Graphics/Pipeline/InputLayout/InputElementSemantic.h"

namespace MAGE
{
    struct InputElement
    {
        TextureFormat Format;
        InputElementSemantic Semantic;
    };
}