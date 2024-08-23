#pragma once

namespace MAGE {
    enum class LogicOperation {
        Clear,
        Set,
        Copy,
        CopyInverted,
        NoOp,
        Invert,
        And,
        NotAnd,
        Or,
        Nor,
        Xor,
        Equal,
        AndReverse,
        AndInverted,
        OrReverse,
        OrInverted
    };
}
