#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
    class LinuxGuid
    {
    public:
        static void GenerateGuid(Guid& guidOut);
    };
}