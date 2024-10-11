#include "VulkanDevice.h"

#include "../Core/VkAssert.h"
#include "../Instance/VulkanInstance.h"
#include "../Queue/VulkanQueue.h"

namespace MAGE
{
	VulkanDevice::VulkanDevice(const DeviceProps& desc, VulkanInstance* instance) : m_props(desc), m_device(VK_NULL_HANDLE)
		, m_adapter(instance->GetAdapter()), m_instance(instance->GetInstance())
	{
		u32 queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_adapter, &queueFamilyCount, nullptr);

		Vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_adapter, &queueFamilyCount, queueFamilies.data());

		i32 index = 0;
		for (auto& prop : queueFamilies)
		{
			if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT && m_graphicsQueueFamily.m_familyIndex == 255)
			{
				m_graphicsQueueFamily.m_familyIndex = index;
				m_graphicsQueueFamily.m_queueCount = prop.queueCount;
				m_graphicsQueueFamily.m_requestedCount = m_graphicsQueueFamily.m_requestedCount > prop.queueCount ? prop.queueCount : 
					desc.m_graphicsQueueCount;
			}
			else if (prop.queueFlags & VK_QUEUE_COMPUTE_BIT && m_computeQueueFamily.m_familyIndex == 255)
			{
				m_computeQueueFamily.m_familyIndex = index;
				m_computeQueueFamily.m_queueCount = prop.queueCount;
				m_computeQueueFamily.m_requestedCount = m_computeQueueFamily.m_requestedCount > prop.queueCount ? prop.queueCount :
					desc.m_computeQueueCount;
			}
			else if (prop.queueFlags & VK_QUEUE_TRANSFER_BIT && m_transferQueueFamily.m_familyIndex == 255)
			{
				m_transferQueueFamily.m_familyIndex = index;
				m_transferQueueFamily.m_queueCount = prop.queueCount;
				m_transferQueueFamily.m_requestedCount = m_transferQueueFamily.m_requestedCount > prop.queueCount ? prop.queueCount :
					desc.m_transferQueueCount;
			}
			index++;
		}

		Vector<f32> queuePriorities(16, 1.0f);
		Vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		if (m_graphicsQueueFamily.m_familyIndex != 255)
		{
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = m_graphicsQueueFamily.m_familyIndex;
			queueCreateInfo.queueCount = m_props.m_computeQueueCount;
			queueCreateInfo.pQueuePriorities = queuePriorities.data();
			queueCreateInfos.push_back(queueCreateInfo);
		}

		if (m_computeQueueFamily.m_familyIndex != 255)
		{
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = m_computeQueueFamily.m_familyIndex;
			queueCreateInfo.queueCount = m_computeQueueFamily.m_requestedCount;
			queueCreateInfo.pQueuePriorities = queuePriorities.data();
			queueCreateInfos.push_back(queueCreateInfo);
		}

		if (m_transferQueueFamily.m_familyIndex != 255)
		{
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = m_transferQueueFamily.m_familyIndex;
			queueCreateInfo.queueCount = m_transferQueueFamily.m_requestedCount;
			queueCreateInfo.pQueuePriorities = queuePriorities.data();
			queueCreateInfos.push_back(queueCreateInfo);
		}

		// Add all the wanted device extensions
		Vector<const char*> extensions;
		extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		extensions.push_back(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
		extensions.push_back(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME);
		extensions.push_back(VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME);

		//Check if the device supports the extensions
		u32 extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(m_adapter, nullptr, &extensionCount, nullptr);
		Vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(m_adapter, nullptr, &extensionCount, availableExtensions.data());

		// Get all the device features related to adapter
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(m_adapter, &deviceFeatures);

		VkPhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT dynamicRenderingUnusedAttachmentsFeatures = {};
		dynamicRenderingUnusedAttachmentsFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_FEATURES_EXT;
		dynamicRenderingUnusedAttachmentsFeatures.pNext = nullptr;
		dynamicRenderingUnusedAttachmentsFeatures.dynamicRenderingUnusedAttachments = VK_TRUE;

		// Add bindless descriptor support
		VkPhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures = {};
		descriptorIndexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
		descriptorIndexingFeatures.pNext = &dynamicRenderingUnusedAttachmentsFeatures;
		descriptorIndexingFeatures.runtimeDescriptorArray = VK_TRUE;
		descriptorIndexingFeatures.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
		descriptorIndexingFeatures.descriptorBindingPartiallyBound = VK_TRUE;
		descriptorIndexingFeatures.descriptorBindingUpdateUnusedWhilePending = VK_TRUE;
		descriptorIndexingFeatures.descriptorBindingSampledImageUpdateAfterBind = VK_TRUE;
		descriptorIndexingFeatures.descriptorBindingStorageImageUpdateAfterBind = VK_TRUE;
		descriptorIndexingFeatures.descriptorBindingUniformBufferUpdateAfterBind = VK_TRUE;
		descriptorIndexingFeatures.descriptorBindingStorageBufferUpdateAfterBind = VK_TRUE;

		// Add dynamic rendering support
		VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeatures = {};
		dynamicRenderingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
		dynamicRenderingFeatures.pNext = &descriptorIndexingFeatures;
		dynamicRenderingFeatures.dynamicRendering = VK_TRUE;

		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
		deviceCreateInfo.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size());
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
		deviceCreateInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = extensions.data();
		deviceCreateInfo.pNext = &dynamicRenderingFeatures;

		ErrorUtils::VkAssert(vkCreateDevice(m_adapter, &deviceCreateInfo, nullptr, &m_device), "VulkanDevice");

		m_graphicsQueueFamily.FillQueues(m_device);
		m_computeQueueFamily.FillQueues(m_device);
		m_transferQueueFamily.FillQueues(m_device);
	}

	VulkanDevice::~VulkanDevice()
	{
		vkDestroyDevice(m_device, nullptr);
	}

	Shared<VulkanQueue> VulkanDevice::CreateQueue(VkQueueFlags queueType)
	{
		switch (queueType)
		{
		case VK_QUEUE_GRAPHICS_BIT:
		{
			QueueProps prop = {};
			prop.m_familyIndex = m_graphicsQueueFamily.m_familyIndex;
			prop.m_flags = queueType;
			prop.m_queue = m_graphicsQueueFamily.GetFreeQueue();
			return MakeShared<VulkanQueue>(prop, this);
		}
		case VK_QUEUE_COMPUTE_BIT:
		{
			QueueProps prop = {};
			prop.m_familyIndex = m_computeQueueFamily.m_familyIndex;
			prop.m_flags = queueType;
			prop.m_queue = m_computeQueueFamily.GetFreeQueue();
			return MakeShared<VulkanQueue>(prop, this);
		}
		case VK_QUEUE_TRANSFER_BIT:
		{
			QueueProps prop = {};
			prop.m_familyIndex = m_transferQueueFamily.m_familyIndex;
			prop.m_flags = queueType;
			prop.m_queue = m_transferQueueFamily.GetFreeQueue();
			return MakeShared<VulkanQueue>(prop, this);
		}
		default:
			return nullptr;
		}
	}
}
