#include "VDescLayout.h"

#include "../Core/VkAssert.h"
#include "../Device/VDevice.h"

namespace MAGE
{
	VDescLayout::VDescLayout(const DescLayoutProps& desc, VDevice* device) : VkObject(device), m_props(desc)
	{
		vector<VkDescriptorSetLayoutBinding> bindings(desc.bindings.size());

		for (u32 i = 0; i < desc.bindings.size(); i++)
		{
			bindings[i].binding = desc.bindings[i].binding;
			bindings[i].descriptorType = desc.bindings[i].type;
			bindings[i].descriptorCount = desc.bindings[i].count;
			bindings[i].stageFlags = desc.bindings[i].stageFlags;
			bindings[i].pImmutableSamplers = nullptr;
		}

		VkDescriptorSetLayoutCreateInfo createInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
		createInfo.bindingCount = static_cast<u32>(bindings.size());
		createInfo.pBindings = bindings.data();
		createInfo.flags = desc.createFlags;

		ErrorUtils::VkAssert(vkCreateDescriptorSetLayout(m_rootDevice->GetVkDevice(), &createInfo, nullptr, &m_layout), "VDescriptorLayout");
	}

	VDescLayout::~VDescLayout()
	{
		Destroy();
	}

	void VDescLayout::Destroy()
	{
		if (m_layout != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(m_rootDevice->GetVkDevice(), m_layout, nullptr);
			m_layout = VK_NULL_HANDLE;
		}
	}
}