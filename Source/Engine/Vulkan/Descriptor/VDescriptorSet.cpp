#include "VDescriptorSet.h"

#include "Engine/Vulkan/Descriptor/VDescriptorLayout.h"
#include "Engine/Vulkan/Descriptor/VDescriptorPool.h"

#include "Engine/Platform/PlatformErrorMessage.h"

namespace MAGE
{
	VDescriptorSet::VDescriptorSet(const DescriptorSetDesc& desc, VDevice* pDevice) : DescriptorSet(desc, pDevice),
		mVkDescriptorSet(VK_NULL_HANDLE), mDevice(pDevice->GetVkDevice())
	{
		VkDescriptorSetLayout layout = desc.pLayout->GetAs<VDescriptorLayout>()->GetVkDescriptorLayout();
		VkDescriptorPool pool = desc.pOwnerPool->GetAs<VDescriptorPool>()->GetVkDescriptorPool();

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &layout;
		allocInfo.pNext = nullptr;

		Helpers::MageAssert(vkAllocateDescriptorSets(mDevice, &allocInfo, &mVkDescriptorSet) == VK_SUCCESS, "VulkanLayout", "Failed to allocate descriptor set");

		mVkDescriptorPool = pool;
		mVkDescriptorLayout = layout;
	}
}
