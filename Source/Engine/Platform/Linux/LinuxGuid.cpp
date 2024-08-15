#include "LinuxGuid.h"

#if defined(MAGE_LINUX)
#include <uuid/uuid.h>
#endif 
namespace MAGE
{
    void LinuxGuid::GenerateGuid(Guid& guidOut)
    {
#if defined(MAGE_LINUX)
        uuid_t newGuid;
        uuid_generate(newGuid);

        char str[37];
        uuid_unparse(newGuid, str);
        guidOut = Guid(str);
#endif
    };
}