#include "LinuxGuid.h"

#include <uuid/uuid.h>

namespace MAGE
{
    void LinuxGuid::GenerateGuid(Guid& guidOut)
    {
        uuid_t newGuid;
        uuid_generate(newGuid);

        char str[37];
        uuid_unparse(newGuid, str);
        guidOut = Guid(str);
    };
}