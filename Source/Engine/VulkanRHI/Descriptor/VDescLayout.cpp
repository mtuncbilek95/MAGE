#include "VDescLayout.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Device/VDevice.h"
#include "../Descriptor/VDescBuffer.h"

namespace MAGE
{
	VDescLayout::VDescLayout(const DescLayoutProps& desc, VDevice* device) : VObject(device), 
		m_props(desc), m_layout(VK_NULL_HANDLE)
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

		ErrorUtils::VkAssert(vkCreateDescriptorSetLayout(m_rootDevice->GetDevice(), &layoutInfo, nullptr, &m_layout), "VDescLayout");
	}

	VDescLayout::~VDescLayout()
	{
		Destroy();
	}

	Owned<VDescBuffer> VDescLayout::CreateDescBuffer(u32 bufferCount, VkBufferUsageFlags flags)
	{
		DescBufferProps prop = { bufferCount, this, flags };
		return MakeOwned<VDescBuffer>(prop, m_rootDevice);
	}

	void VDescLayout::Destroy()
	{
		if (m_layout != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(m_rootDevice->GetDevice(), m_layout, nullptr);
			m_layout = VK_NULL_HANDLE;
		}
	}
}
