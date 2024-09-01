#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Pipeline/InputLayout/InputBindingStepRate.h"
#include "Engine/Graphics/Pipeline/InputLayout/InputElement.h"

namespace MAGE
{
    struct InputBinding
    {
        InputBindingStepRate StepRate;
        Vector<InputElement> Elements;
    };
}