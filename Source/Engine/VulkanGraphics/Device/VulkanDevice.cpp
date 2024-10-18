#include "VulkanDevice.h"

#include "../Core/VkAssert.h"
#include "../Core/VkFunctions.h"
#include "../Instance/VulkanInstance.h"
#include "../Queue/VulkanQueue.h"
#include "../Sync/VulkanSemaphore.h"
#include "../Sync/VulkanFence.h"
#include "../Command/VulkanCmdBuffer.h"

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
					desc.graphicsQueueCount;
			}
			else if (prop.queueFlags & VK_QUEUE_COMPUTE_BIT && m_computeQueueFamily.m_familyIndex == 255)
			{
				m_computeQueueFamily.m_familyIndex = index;
				m_computeQueueFamily.m_queueCount = prop.queueCount;
				m_computeQueueFamily.m_requestedCount = m_computeQueueFamily.m_requestedCount > prop.queueCount ? prop.queueCount :
					desc.computeQueueCount;
			}
			else if (prop.queueFlags & VK_QUEUE_TRANSFER_BIT && m_transferQueueFamily.m_familyIndex == 255)
			{
				m_transferQueueFamily.m_familyIndex = index;
				m_transferQueueFamily.m_queueCount = prop.queueCount;
				m_transferQueueFamily.m_requestedCount = m_transferQueueFamily.m_requestedCount > prop.queueCount ? prop.queueCount :
					desc.transferQueueCount;
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
			queueCreateInfo.queueCount = m_graphicsQueueFamily.m_requestedCount;
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

		struct ExtensionEntry
		{
			const char* m_name;
			bool m_support;
		};

		// Define the wanted extensions
		Vector<ExtensionEntry> extensions;
		Vector<const char*> workingExtensions;
		extensions.push_back({ VK_KHR_SWAPCHAIN_EXTENSION_NAME, false });
		extensions.push_back({ VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME, false });
		extensions.push_back({ VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME, false });
		extensions.push_back({ VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME, false });
		extensions.push_back({ VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME, false });
		extensions.push_back({ VK_KHR_MAINTENANCE3_EXTENSION_NAME, false });
		extensions.push_back({ VK_KHR_MAINTENANCE_5_EXTENSION_NAME, false });
		extensions.push_back({ VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME, false });

		//Check if the device supports the extensions
		u32 extensionCount = 0;
		ErrorUtils::VkAssert(vkEnumerateDeviceExtensionProperties(m_adapter, nullptr, &extensionCount, nullptr), "VulkanDevice");
		Vector<VkExtensionProperties> availableExtensions(extensionCount);
		ErrorUtils::VkAssert(vkEnumerateDeviceExtensionProperties(m_adapter, nullptr, &extensionCount, availableExtensions.data()), "VulkanDevice");

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

		for (auto& ext : workingExtensions)
			spdlog::debug("{} is currently on run for VkDevice.", ext);

		ErrorUtils::LogAssert(brokenExtensions.size() == 0, "VulkanDevice", "Your device does not support the bare minimum extensions. You need at least RTX2060 or equivalent.");

		// Create extensions
		VkPhysicalDeviceDescriptorIndexingFeatures descriptorIndexing = {};
		descriptorIndexing.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
		descriptorIndexing.runtimeDescriptorArray = VK_TRUE;
		descriptorIndexing.descriptorBindingPartiallyBound = VK_TRUE;
		descriptorIndexing.descriptorBindingUpdateUnusedWhilePending = VK_TRUE;
		descriptorIndexing.shaderInputAttachmentArrayDynamicIndexing = VK_TRUE;
		descriptorIndexing.shaderUniformTexelBufferArrayDynamicIndexing = VK_TRUE;
		descriptorIndexing.shaderStorageTexelBufferArrayDynamicIndexing = VK_TRUE;
		descriptorIndexing.shaderUniformBufferArrayNonUniformIndexing = VK_TRUE;
		descriptorIndexing.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
		descriptorIndexing.shaderStorageBufferArrayNonUniformIndexing = VK_TRUE;
		descriptorIndexing.shaderStorageImageArrayNonUniformIndexing = VK_TRUE;
		descriptorIndexing.shaderInputAttachmentArrayNonUniformIndexing = VK_TRUE;
		descriptorIndexing.shaderUniformTexelBufferArrayNonUniformIndexing = VK_TRUE;
		descriptorIndexing.shaderStorageTexelBufferArrayNonUniformIndexing = VK_TRUE;
		descriptorIndexing.descriptorBindingUniformBufferUpdateAfterBind = VK_TRUE;
		descriptorIndexing.descriptorBindingSampledImageUpdateAfterBind = VK_TRUE;
		descriptorIndexing.descriptorBindingStorageImageUpdateAfterBind = VK_TRUE;
		descriptorIndexing.descriptorBindingStorageBufferUpdateAfterBind = VK_TRUE;
		descriptorIndexing.descriptorBindingUniformTexelBufferUpdateAfterBind = VK_TRUE;
		descriptorIndexing.descriptorBindingStorageTexelBufferUpdateAfterBind = VK_TRUE;
		descriptorIndexing.descriptorBindingUpdateUnusedWhilePending = VK_TRUE;
		descriptorIndexing.descriptorBindingPartiallyBound = VK_TRUE;
		descriptorIndexing.descriptorBindingVariableDescriptorCount = VK_TRUE;
		descriptorIndexing.runtimeDescriptorArray = VK_TRUE;

		VkPhysicalDeviceBufferDeviceAddressFeatures bufferDeviceAddress = {};
		bufferDeviceAddress.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
		bufferDeviceAddress.bufferDeviceAddress = VK_TRUE;
		bufferDeviceAddress.bufferDeviceAddressCaptureReplay = VK_TRUE;
		bufferDeviceAddress.bufferDeviceAddressMultiDevice = VK_TRUE;
		bufferDeviceAddress.pNext = &descriptorIndexing;

		VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRendering = {};
		dynamicRendering.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
		dynamicRendering.dynamicRendering = VK_TRUE;
		dynamicRendering.pNext = &bufferDeviceAddress;

		VkPhysicalDeviceDescriptorBufferFeaturesEXT descriptorBuffer = {};
		descriptorBuffer.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_FEATURES_EXT;
		descriptorBuffer.descriptorBuffer = VK_TRUE;
		descriptorBuffer.pNext = &dynamicRendering;

		// Get all the device features related to adapter
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(m_adapter, &deviceFeatures);

		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
		deviceCreateInfo.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size());
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
		deviceCreateInfo.enabledExtensionCount = static_cast<u32>(workingExtensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = workingExtensions.data();
		deviceCreateInfo.pNext = &descriptorBuffer;

		ErrorUtils::VkAssert(vkCreateDevice(m_adapter, &deviceCreateInfo, nullptr, &m_device), "VulkanDevice");

		m_graphicsQueueFamily.FillQueues(m_device);
		m_computeQueueFamily.FillQueues(m_device);
		m_transferQueueFamily.FillQueues(m_device);

		// Load the function pointers
		LoadVulkanExtensionFunctions(m_device);
	}

	VulkanDevice::~VulkanDevice()
	{
		vkDestroyDevice(m_device, nullptr);
	}

	Owned<VulkanQueue> VulkanDevice::CreateQueue(VkQueueFlags queueType)
	{
		switch (queueType)
		{
		case VK_QUEUE_GRAPHICS_BIT:
		{
			QueueProps prop = {};
			prop.m_familyIndex = m_graphicsQueueFamily.m_familyIndex;
			prop.m_flags = queueType;
			prop.m_queue = m_graphicsQueueFamily.GetFreeQueue();
			return MakeOwned<VulkanQueue>(prop, this);
		}
		case VK_QUEUE_COMPUTE_BIT:
		{
			QueueProps prop = {};
			prop.m_familyIndex = m_computeQueueFamily.m_familyIndex;
			prop.m_flags = queueType;
			prop.m_queue = m_computeQueueFamily.GetFreeQueue();
			return MakeOwned<VulkanQueue>(prop, this);
		}
		case VK_QUEUE_TRANSFER_BIT:
		{
			QueueProps prop = {};
			prop.m_familyIndex = m_transferQueueFamily.m_familyIndex;
			prop.m_flags = queueType;
			prop.m_queue = m_transferQueueFamily.GetFreeQueue();
			return MakeOwned<VulkanQueue>(prop, this);
		}
		default:
			throw std::runtime_error("Queue type not supported!");
		}
	}

	Owned<VulkanSemaphore> VulkanDevice::CreateSyncSemaphore()
	{
		return MakeOwned<VulkanSemaphore>(this);
	}

	Owned<VulkanFence> VulkanDevice::CreateSyncFence(bool signaled)
	{
		return MakeOwned<VulkanFence>(signaled, this);
	}

	u32 VulkanDevice::FindMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties) const
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_adapter, &memProperties);

		for (u32 i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		ErrorUtils::LogAssert(false, "VulkanDevice", "Failed to find suitable memory type!");
		return 0;
	}

	void VulkanDevice::WaitForIdle() const
	{
		ErrorUtils::VkAssert(vkDeviceWaitIdle(m_device), "VulkanDevice");
	}

	void VulkanDevice::WaitForFence(VulkanFence* pFence) const
	{
		VkFence fence = pFence->GetFence();
		ErrorUtils::VkAssert(vkWaitForFences(m_device, 1, &fence, VK_TRUE, UINT64_MAX), "VulkanDevice");
	}

	void VulkanDevice::ResetFence(VulkanFence* pFence) const
	{
		VkFence fence = pFence->GetFence();
		ErrorUtils::VkAssert(vkResetFences(m_device, 1, &fence), "VulkanDevice");
	}

	void VulkanDevice::SubmitQueue(VulkanQueue* pQueue, VulkanCmdBuffer* pCmdBuffer, VulkanSemaphore* waitSemaphore, VulkanSemaphore* signalSemaphore, VulkanFence* pFence, VkPipelineStageFlags flags) const
	{
		VkSemaphore waitSem = waitSemaphore ? waitSemaphore->GetSemaphore() : VK_NULL_HANDLE;
		VkSemaphore signalSem = signalSemaphore ? signalSemaphore->GetSemaphore() : VK_NULL_HANDLE;
		VkCommandBuffer cmdBuffer = pCmdBuffer->GetCmdBuffer();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = waitSemaphore ? 1 : 0;
		submitInfo.pWaitSemaphores = &waitSem;
		submitInfo.pWaitDstStageMask = &flags;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmdBuffer;
		submitInfo.signalSemaphoreCount = signalSemaphore ? 1 : 0;
		submitInfo.pSignalSemaphores = &signalSem;

		ErrorUtils::VkAssert(vkQueueSubmit(pQueue->GetQueue(), 1, &submitInfo, pFence ? pFence->GetFence() : VK_NULL_HANDLE), "VulkanDevice");
	}
}
