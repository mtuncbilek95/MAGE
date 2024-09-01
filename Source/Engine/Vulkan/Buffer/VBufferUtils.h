#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Buffer/GraphicsBufferUsage.h"

#include <vulkan/vulkan.h>

namespace MAGE
{
	namespace VkUtils
	{
		static VkBufferUsageFlags GetVKBufferUsage(GraphicsBufferUsage usage)
		{
			VkBufferUsageFlags flags = 0;

			if (usage & GraphicsBufferUsage::GBU_Vertex)
				flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			else if (usage & GraphicsBufferUsage::GBU_Index)
				flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			else if (usage & GraphicsBufferUsage::GBU_Uniform)
				flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			else if (usage & GraphicsBufferUsage::GBU_Storage)
				flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

			if (usage & GraphicsBufferUsage::GBU_TransferDst)
				flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			else if (usage & GraphicsBufferUsage::GBU_TransferSrc)
				flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

			return flags;
		}
	}
}
