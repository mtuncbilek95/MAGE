#include "VQueue.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Device/VDevice.h"

namespace MAGE
{
	VQueue::VQueue(const QueueProps& desc, VDevice* device) : VObject(device), m_props(desc)
	{
	}

	VQueue::~VQueue()
	{
	}

	void VQueue::Destroy()
	{
	}
}
