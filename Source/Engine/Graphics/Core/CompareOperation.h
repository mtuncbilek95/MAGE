#pragma once

#include <Engine/Core/Core.h>

namespace MAGE {
    enum class CompareOperation : u32 {
        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        NotEqual,
        GreaterEqual,
        CO_Always
    };

    GENERATE_ENUM_FLAG(MAGE::CompareOperation, u32);
}
