#include "VulkanDescLayout.h"

#include "../Core/VkAssert.h"
#include "../Device/VulkanDevice.h"

namespace MAGE
{
	VulkanDescLayout::VulkanDescLayout(const DescLayoutProps& desc, VulkanDevice* device) : m_props(desc), m_deviceRef(device)
	{
		Vector<VkDescriptorSetLayoutBinding> bindings(desc.bindings.size());

		for (u32 i = 0; i < desc.bindings.size(); i++)
		{
			bindings[i].binding = desc.bindings[i].binding;
			bindings[i].descriptorType = desc.bindings[i].type;
			bindings[i].descriptorCount = desc.bindings[i].count;
			bindings[i].stageFlags = desc.bindings[i].stageFlags;
			bindings[i].pImmutableSamplers = nullptr;
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<u32>(bindings.size());
		layoutInfo.pBindings = bindings.data();
		layoutInfo.flags = desc.initFlags;

		ErrorUtils::VkAssert(vkCreateDescriptorSetLayout(m_deviceRef->GetDevice(), &layoutInfo, nullptr, &m_layout), "VulkanDescLayout");
	}

	VulkanDescLayout::~VulkanDescLayout()
	{
		vkDestroyDescriptorSetLayout(m_deviceRef->GetDevice(), m_layout, nullptr);
	}
}
