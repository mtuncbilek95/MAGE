#include "VQueue.h"

namespace MAGE
{
	VQueue::VQueue(const GraphicsQueueDesc& desc, VkQueue queue, GraphicsDevice* pDevice) : GraphicsQueue(desc, pDevice), mQueue(queue)
	{
	}
}
