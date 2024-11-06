#include "VDevice.h"
#include "../Instance/VInstance.h"
#include "../Queue/VQueue.h"

#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	VDevice::VDevice(const DeviceProps& desc, VInstance* instance) : m_instance(instance->GetVkInstance()), 
		m_adapter(instance->GetVkAdapter()), m_loader(instance->GetVkLoader())
	{
		u32 queueFamilyCount = 0;
		m_adapter.getQueueFamilyProperties(&queueFamilyCount, nullptr);

		Vector<vk::QueueFamilyProperties> queueFamilies(queueFamilyCount);
		m_adapter.getQueueFamilyProperties(&queueFamilyCount, queueFamilies.data());

		i32 index = 0;
		for (auto& prop : queueFamilies)
		{
			if (prop.queueFlags & vk::QueueFlagBits::eGraphics && m_graphicsQueueFamily.m_familyIndex == 255)
			{
				m_graphicsQueueFamily.m_familyIndex = index;
				m_graphicsQueueFamily.m_queueCount = prop.queueCount;
				m_graphicsQueueFamily.m_requestedCount = m_graphicsQueueFamily.m_requestedCount > prop.queueCount ? prop.queueCount :
					desc.graphicsQueueCount;
			}
			else if (prop.queueFlags & vk::QueueFlagBits::eCompute && m_computeQueueFamily.m_familyIndex == 255)
			{
				m_computeQueueFamily.m_familyIndex = index;
				m_computeQueueFamily.m_queueCount = prop.queueCount;
				m_computeQueueFamily.m_requestedCount = m_computeQueueFamily.m_requestedCount > prop.queueCount ? prop.queueCount :
					desc.computeQueueCount;
			}
			else if (prop.queueFlags & vk::QueueFlagBits::eTransfer && m_transferQueueFamily.m_familyIndex == 255)
			{
				m_transferQueueFamily.m_familyIndex = index;
				m_transferQueueFamily.m_queueCount = prop.queueCount;
				m_transferQueueFamily.m_requestedCount = m_transferQueueFamily.m_requestedCount > prop.queueCount ? prop.queueCount :
					desc.transferQueueCount;
			}
			index++;
		}

		Vector<f32> priorities(16, 1.f);
		Vector<vk::DeviceQueueCreateInfo> queueInfos;

		if (m_graphicsQueueFamily.m_familyIndex != 255)
		{
			vk::DeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.queueFamilyIndex = m_graphicsQueueFamily.m_familyIndex;
			queueCreateInfo.queueCount = m_graphicsQueueFamily.m_requestedCount;
			queueCreateInfo.pQueuePriorities = priorities.data();
			queueInfos.push_back(queueCreateInfo);
		}

		if (m_computeQueueFamily.m_familyIndex != 255)
		{
			vk::DeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.queueFamilyIndex = m_computeQueueFamily.m_familyIndex;
			queueCreateInfo.queueCount = m_computeQueueFamily.m_requestedCount;
			queueCreateInfo.pQueuePriorities = priorities.data();
			queueInfos.push_back(queueCreateInfo);
		}

		if (m_transferQueueFamily.m_familyIndex != 255)
		{
			vk::DeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.queueFamilyIndex = m_transferQueueFamily.m_familyIndex;
			queueCreateInfo.queueCount = m_transferQueueFamily.m_requestedCount;
			queueCreateInfo.pQueuePriorities = priorities.data();
			queueInfos.push_back(queueCreateInfo);
		}

		struct ExtensionEntry
		{
			const char* m_name;
			bool m_support;
		};

		// Define the wanted extensions
		Vector<ExtensionEntry> extensions;
		Vector<const char*> workingExtensions;
		extensions.push_back({ VK_KHR_SWAPCHAIN_EXTENSION_NAME, false });
		extensions.push_back({ VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME, false });
		extensions.push_back({ VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME, false });
		extensions.push_back({ VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME, false });
		extensions.push_back({ VK_KHR_MAINTENANCE3_EXTENSION_NAME, false });
		extensions.push_back({ VK_KHR_MAINTENANCE_5_EXTENSION_NAME, false });
		extensions.push_back({ VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME, false });
		extensions.push_back({ VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME, false });

		//Check if the device supports the extensions
		u32 extensionCount = 0;
		ErrorUtils::VkAssert(m_adapter.enumerateDeviceExtensionProperties(nullptr, &extensionCount, nullptr), "VDevice");
		Vector<vk::ExtensionProperties> availableExtensions(extensionCount);
		ErrorUtils::VkAssert(m_adapter.enumerateDeviceExtensionProperties(nullptr, &extensionCount, availableExtensions.data()), "VDevice");

		for (usize i = 0; i < extensions.size(); ++i)
		{
			for (auto& extension : availableExtensions)
			{
				if (strcmp(extensions[i].m_name, extension.extensionName) == 0)
				{
					extensions[i].m_support = true;
					workingExtensions.push_back(extensions[i].m_name);
					break;
				}
			}
		}

		Vector<const char*> brokenExtensions;

		for (auto& extension : extensions)
		{
			if (extension.m_support)
				workingExtensions.push_back(extension.m_name);
			else
				brokenExtensions.push_back(extension.m_name);
		}

		for (auto& ext : brokenExtensions)
			spdlog::error("{} has no support on this hardware specs", ext);

		vk::PhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT unusedFeatures = {};
		unusedFeatures.dynamicRenderingUnusedAttachments = true;

		vk::PhysicalDeviceDescriptorIndexingFeatures indexFeatures = vk::PhysicalDeviceDescriptorIndexingFeatures();
		indexFeatures.runtimeDescriptorArray = true;
		indexFeatures.descriptorBindingPartiallyBound = true;
		indexFeatures.descriptorBindingUpdateUnusedWhilePending = true;
		indexFeatures.shaderInputAttachmentArrayDynamicIndexing = true;
		indexFeatures.shaderUniformTexelBufferArrayDynamicIndexing = true;
		indexFeatures.shaderStorageTexelBufferArrayDynamicIndexing = true;
		indexFeatures.shaderUniformBufferArrayNonUniformIndexing = true;
		indexFeatures.shaderSampledImageArrayNonUniformIndexing = true;
		indexFeatures.shaderStorageBufferArrayNonUniformIndexing = true;
		indexFeatures.shaderStorageImageArrayNonUniformIndexing = true;
		indexFeatures.shaderInputAttachmentArrayNonUniformIndexing = true;
		indexFeatures.shaderUniformTexelBufferArrayNonUniformIndexing = true;
		indexFeatures.shaderStorageTexelBufferArrayNonUniformIndexing = true;
		indexFeatures.descriptorBindingUniformBufferUpdateAfterBind = true;
		indexFeatures.descriptorBindingSampledImageUpdateAfterBind = true;
		indexFeatures.descriptorBindingStorageImageUpdateAfterBind = true;
		indexFeatures.descriptorBindingStorageBufferUpdateAfterBind = true;
		indexFeatures.descriptorBindingUniformTexelBufferUpdateAfterBind = true;
		indexFeatures.descriptorBindingStorageTexelBufferUpdateAfterBind = true;
		indexFeatures.descriptorBindingUpdateUnusedWhilePending = true;
		indexFeatures.descriptorBindingPartiallyBound = true;
		indexFeatures.descriptorBindingVariableDescriptorCount = true;
		indexFeatures.runtimeDescriptorArray = true;
		indexFeatures.pNext = &unusedFeatures;

		vk::PhysicalDeviceBufferDeviceAddressFeatures addressFeatures = {};
		addressFeatures.bufferDeviceAddress = true;
		addressFeatures.bufferDeviceAddressCaptureReplay = true;
		addressFeatures.bufferDeviceAddressMultiDevice = true;
		addressFeatures.pNext = &indexFeatures;

		vk::PhysicalDeviceDescriptorBufferFeaturesEXT descriptorBuffer = {};
		descriptorBuffer.descriptorBuffer = true;
		descriptorBuffer.pNext = &addressFeatures;

		vk::PhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeatures = {};
		dynamicRenderingFeatures.dynamicRendering = true;
		dynamicRenderingFeatures.pNext = &descriptorBuffer;

		vk::PhysicalDeviceFeatures deviceFeatures;
		m_adapter.getFeatures(&deviceFeatures);

		vk::DeviceCreateInfo deviceInfo = {};
		deviceInfo.pQueueCreateInfos = queueInfos.data();
		deviceInfo.queueCreateInfoCount = static_cast<u32>(queueInfos.size());
		deviceInfo.pEnabledFeatures = &deviceFeatures;
		deviceInfo.enabledExtensionCount = static_cast<u32>(workingExtensions.size());
		deviceInfo.ppEnabledExtensionNames = workingExtensions.data();
		deviceInfo.pNext = &dynamicRenderingFeatures;

		ErrorUtils::VkAssert(m_adapter.createDevice(&deviceInfo, nullptr, &m_device), "VDevice");
		
		m_graphicsQueueFamily.FillQueues(m_device);
		m_computeQueueFamily.FillQueues(m_device);
		m_transferQueueFamily.FillQueues(m_device);

		m_loader.init(m_device);

		m_memAllocator = MakeOwned<MemoryAllocator>(this);
	}

	VDevice::~VDevice()
	{
		Destroy();
	}

	u32 VDevice::FindMemoryType(vk::MemoryPropertyFlags properties)
	{
		vk::PhysicalDeviceMemoryProperties memProperties;
		m_adapter.getMemoryProperties(&memProperties);

		for (u32 i = 0; i < memProperties.memoryTypeCount; i++)
			if ((memProperties.memoryTypes[i].propertyFlags & properties) == properties)
				return i;

		return u32_max;
	}

	Owned<VQueue> VDevice::CreateQueue(vk::QueueFlagBits queueType)
	{
		switch (queueType)
		{
		case vk::QueueFlagBits::eGraphics:
		{
			QueueProps prop = {};
			prop.familyIndex = m_graphicsQueueFamily.m_familyIndex;
			prop.flags = queueType;
			return MakeOwned<VQueue>(prop, m_graphicsQueueFamily.GetFreeQueue(), this);
		}
		case vk::QueueFlagBits::eCompute:
		{
			QueueProps prop = {};
			prop.familyIndex = m_computeQueueFamily.m_familyIndex;
			prop.flags = queueType;
			return MakeOwned<VQueue>(prop, m_computeQueueFamily.GetFreeQueue(), this);
		}
		case vk::QueueFlagBits::eTransfer:
		{
			QueueProps prop = {};
			prop.familyIndex = m_transferQueueFamily.m_familyIndex;
			prop.flags = queueType;
			return MakeOwned<VQueue>(prop, m_transferQueueFamily.GetFreeQueue(), this);
		}
		default:
			throw std::runtime_error("Queue type not supported!");
		}
	}

	void VDevice::Destroy()
	{
		m_memAllocator->Destroy();

		if (m_device != VK_NULL_HANDLE)
		{
			m_device.destroy();
			m_device = VK_NULL_HANDLE;
		}
	}
}
