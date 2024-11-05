#include "VkObject.h"
#include "../RHI/Device/VDevice.h"

namespace MAGE
{
	VkObject::VkObject(VDevice* device) : m_rootDevice(device) {}
}
