#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Queue/GraphicsQueue.h>

#include <vulkan/vulkan.h>

namespace MAGE
{
	class VDevice;
	class VQueue : public GraphicsQueue
	{
	public:
		VQueue(const GraphicsQueueDesc& desc, VkQueue queue, GraphicsDevice* pDevice);
		~VQueue() override = default;

		VkQueue GetVkQueue() const { return mQueue; }

	private:
		VkQueue mQueue;
	};
}
