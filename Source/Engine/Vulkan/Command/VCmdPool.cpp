#include "VCmdPool.h"
#include "../Core/VCoreUtils.h"

#include <Engine/Vulkan/Device/VDevice.h>

namespace MAGE
{
	VCmdPool::VCmdPool(const CmdPoolDesc& desc, VDevice* pDevice) : CmdPool(desc, pDevice),
		mDevice(pDevice->GetVkDevice()), mVkCommandPool(VK_NULL_HANDLE)
	{
		u32 queueFamilyIndex = {};
		switch (desc.PoolType)
		{
		case CmdPoolType::Graphics:
			queueFamilyIndex = pDevice->GetGraphicsQueueFamilyIndex();
			break;
		case CmdPoolType::Compute:
			queueFamilyIndex = pDevice->GetComputeQueueFamilyIndex();
			break;
		case CmdPoolType::Transfer:
			queueFamilyIndex = pDevice->GetTransferQueueFamilyIndex();
			break;
		default:
			CORE_ASSERT(false, "VCmdPool", "Invalid command pool type");
		}

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndex;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.pNext = nullptr;

		CORE_ASSERT(vkCreateCommandPool(mDevice, &poolInfo, nullptr, &mVkCommandPool) == VK_SUCCESS, "VCmdPool", "Failed to create command pool");
	}
}
