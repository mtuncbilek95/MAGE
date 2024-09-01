#include "VDescriptorPool.h"

#include "Engine/Platform/PlatformErrorMessage.h"
#include "Engine/Vulkan/Descriptor/VDescriptorUtils.h"

namespace MAGE
{
	VDescriptorPool::VDescriptorPool(const DescriptorPoolDesc& desc, VDevice* pDevice) : DescriptorPool(desc, pDevice),
		mVkDescriptorPool(VK_NULL_HANDLE), mDevice(pDevice->GetVkDevice())
	{
		Vector<VkDescriptorPoolSize> poolSizes;

		for (auto& poolSize : desc.PoolSizes)
		{
			VkDescriptorPoolSize size = {};
			size.type = static_cast<VkDescriptorType>(poolSize.Type);
			size.descriptorCount = poolSize.Count;

			poolSizes.push_back(size);
		}

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<u32>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = desc.MaxSets;
		poolInfo.flags = VkUtils::GetVkDescriptorPoolFlags(desc.Flags);
		poolInfo.pNext = nullptr;

		Helpers::MageAssert(vkCreateDescriptorPool(mDevice, &poolInfo, nullptr, &mVkDescriptorPool) == VK_SUCCESS, "VDescriptorPool", "Failed to create descriptor pool");
	}
}
