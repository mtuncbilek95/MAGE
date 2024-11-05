#include "VQueue.h"

#include "../Device/VDevice.h"

namespace MAGE
{
	VQueue::VQueue(const QueueProps& desc, vk::Queue queue, VDevice* device) : VkObject(device), m_props(desc), m_queue(queue) {}
	VQueue::~VQueue() { Destroy(); }
	void VQueue::Destroy() {}
}
