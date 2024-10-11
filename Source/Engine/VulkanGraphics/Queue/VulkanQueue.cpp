#include "VulkanQueue.h"

#include "../Device/VulkanDevice.h"

namespace MAGE
{
	VulkanQueue::VulkanQueue(const QueueProps& desc, VulkanDevice* device) : m_props(desc), m_device(device->GetDevice())
	{
	}

	VulkanQueue::~VulkanQueue()
	{
	}
}
