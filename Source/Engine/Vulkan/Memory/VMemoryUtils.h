#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Memory/GraphicsMemoryType.h"
#include "Engine/Graphics/Memory/GraphicsMemoryAccessFlags.h"

#include <vulkan/vulkan.h>

namespace MAGE
{
	namespace VkUtils
	{
		static VkMemoryPropertyFlags GetVkMemoryPropertyFlags(GraphicsMemoryType memoryType)
		{
			VkMemoryPropertyFlags flags = 0;

			if (memoryType & GraphicsMemoryType::GMT_DeviceLocal)
				flags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			if (memoryType & GraphicsMemoryType::GMT_HostVisible)
				flags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			if (memoryType & GraphicsMemoryType::GMT_HostCoherent)
				flags |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			if (memoryType & GraphicsMemoryType::GMT_HostCached)
				flags |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;

			return flags;
		}

		static VkAccessFlags GetVkAccessFlags(GraphicsMemoryAccessFlags flags)
		{
            VkAccessFlags flagsOut = VkAccessFlags();

            if (flags & GraphicsMemoryAccessFlags::GMAF_Unknown)
                return VK_ACCESS_NONE;

            if (flags & GraphicsMemoryAccessFlags::GMAF_IndexRead)
                flagsOut |= VK_ACCESS_INDEX_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::GMAF_VertexAttributeRead)
                flagsOut |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::GMAF_UniformRead)
                flagsOut |= VK_ACCESS_UNIFORM_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::GMAF_InputAttachmentRead)
                flagsOut |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::GMAF_ShaderRead)
                flagsOut |= VK_ACCESS_SHADER_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::GMAF_ShaderWrite)
                flagsOut |= VK_ACCESS_SHADER_WRITE_BIT;
            if (flags & GraphicsMemoryAccessFlags::GMAF_ColorAttachmentRead)
                flagsOut |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::GMAF_ColorAttachmentWrite)
                flagsOut |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            if (flags & GraphicsMemoryAccessFlags::GMAF_DepthStencilAttachmentRead)
                flagsOut |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::GMAF_DepthStencilAttachmentWrite)
                flagsOut |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            if (flags & GraphicsMemoryAccessFlags::GMAF_TransferRead)
                flagsOut |= VK_ACCESS_TRANSFER_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::GMAF_TransferWrite)
                flagsOut |= VK_ACCESS_TRANSFER_WRITE_BIT;
            if (flags & GraphicsMemoryAccessFlags::GMAF_HostRead)
                flagsOut |= VK_ACCESS_HOST_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::GMAF_HostWrite)
                flagsOut |= VK_ACCESS_HOST_WRITE_BIT;
            if (flags & GraphicsMemoryAccessFlags::GMAF_MemoryRead)
                flagsOut |= VK_ACCESS_MEMORY_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::GMAF_MemoryWrite)
                flagsOut |= VK_ACCESS_MEMORY_WRITE_BIT;

            return flagsOut;
		}

	}
}
