#include "PlatformCriticalSection.h"

namespace MAGE
{
	SharedPtr<PlatformCriticalSection> PlatformCriticalSection::Create()
	{
		return SharedPtr<PlatformCriticalSection>();
	}
}
