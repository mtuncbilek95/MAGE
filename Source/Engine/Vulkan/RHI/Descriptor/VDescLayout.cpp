#include "VDescLayout.h"

#include "../Device/VDevice.h"
#include "../Descriptor/VDescBuffer.h"
#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	VDescLayout::VDescLayout(const DescLayoutProps& desc, VDevice* device) : VkObject(device), m_props(desc)
	{
		Vector<vk::DescriptorSetLayoutBinding> bindings(desc.bindings.size());

		for (u32 i = 0; i < desc.bindings.size(); i++)
		{
			bindings[i].binding = desc.bindings[i].binding;
			bindings[i].descriptorType = desc.bindings[i].type;
			bindings[i].descriptorCount = desc.bindings[i].count;
			bindings[i].stageFlags = desc.bindings[i].stageFlags;
			bindings[i].pImmutableSamplers = nullptr;
		}

		vk::DescriptorSetLayoutCreateInfo createInfo = {};
		createInfo.bindingCount = static_cast<u32>(bindings.size());
		createInfo.pBindings = bindings.data();
		createInfo.flags = desc.createFlags;

		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().createDescriptorSetLayout(&createInfo, nullptr, &m_layout), "VDescriptorLayout");
	}

	VDescLayout::~VDescLayout()
	{
		Destroy();
	}

	Owned<VDescBuffer> VDescLayout::CreateDescBuffer(u32 resourceCount, vk::BufferUsageFlags flags)
	{
		DescBufferProps props = {};
		props.layout = this;
		props.resourceCount = resourceCount;
		props.usageFlags = flags;
		return MakeOwned<VDescBuffer>(props, m_rootDevice);
	}

	vk::PhysicalDeviceDescriptorBufferPropertiesEXT VDescLayout::GetBufferProps() const
	{
		vk::PhysicalDeviceDescriptorBufferPropertiesEXT bufferProps = {};
		vk::PhysicalDeviceProperties2 deviceProp2 = {};
		deviceProp2.pNext = &bufferProps;
		m_rootDevice->GetVkAdapter().getProperties2(&deviceProp2);

		return bufferProps;
	}

	void VDescLayout::Destroy()
	{
		if (m_layout != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().destroyDescriptorSetLayout(m_layout, nullptr);
			m_layout = VK_NULL_HANDLE;
		}
	}
}
