#include "VDevice.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Instance/VInstance.h"

namespace MAGE
{
	VDevice::VDevice(const DeviceProps& desc, VInstance* instance) : m_props(desc),
		m_adapter(instance->GetAdapter()), m_instance(instance->GetInstance()), m_device(VK_NULL_HANDLE)
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
		extensions.push_back({ VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME, false });
		extensions.push_back({ VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME, false });
		extensions.push_back({ VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME, false });
		extensions.push_back({ VK_KHR_MAINTENANCE3_EXTENSION_NAME, false });
		extensions.push_back({ VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME, false});
		extensions.push_back({ VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME, false});


		//Check if the device supports the extensions
		u32 extensionCount = 0;
		ErrorUtils::VkAssert(vkEnumerateDeviceExtensionProperties(m_adapter, nullptr, &extensionCount, nullptr), "VDevice");
		Vector<VkExtensionProperties> availableExtensions(extensionCount);
		ErrorUtils::VkAssert(vkEnumerateDeviceExtensionProperties(m_adapter, nullptr, &extensionCount, availableExtensions.data()), "VDevice");

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

		VkPhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT dynamicRenderingUnusedAttachmentsFeatures = {};
		dynamicRenderingUnusedAttachmentsFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_FEATURES_EXT;
		dynamicRenderingUnusedAttachmentsFeatures.pNext = nullptr;
		dynamicRenderingUnusedAttachmentsFeatures.dynamicRenderingUnusedAttachments = VK_TRUE;

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
		descriptorIndexing.pNext = &dynamicRenderingUnusedAttachmentsFeatures;

		VkPhysicalDeviceBufferDeviceAddressFeatures bufferDeviceAddress = {};
		bufferDeviceAddress.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
		bufferDeviceAddress.bufferDeviceAddress = VK_TRUE;
		bufferDeviceAddress.bufferDeviceAddressCaptureReplay = VK_TRUE;
		bufferDeviceAddress.bufferDeviceAddressMultiDevice = VK_TRUE;
		bufferDeviceAddress.pNext = &descriptorIndexing;

		VkPhysicalDeviceDescriptorBufferFeaturesEXT descriptorBuffer = {};
		descriptorBuffer.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_FEATURES_EXT;
		descriptorBuffer.descriptorBuffer = VK_TRUE;
		descriptorBuffer.pNext = &bufferDeviceAddress;

		VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeatures = {};
		dynamicRenderingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
		dynamicRenderingFeatures.pNext = &descriptorBuffer;
		dynamicRenderingFeatures.dynamicRendering = VK_TRUE;

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
		deviceCreateInfo.pNext = &dynamicRenderingFeatures;

		ErrorUtils::VkAssert(vkCreateDevice(m_adapter, &deviceCreateInfo, nullptr, &m_device), "VDevice");

		m_graphicsQueueFamily.FillQueues(m_device);
		m_computeQueueFamily.FillQueues(m_device);
		m_transferQueueFamily.FillQueues(m_device);

		// Load the function pointers
		LoadVulkanDeviceFunctions(m_device);
	}

	VDevice::~VDevice()
	{
		Destroy();
	}

	Owned<VQueue >VDevice::CreateQueue(VkQueueFlags queueType)
	{
		switch (queueType)
		{
		case VK_QUEUE_GRAPHICS_BIT:
		{
			QueueProps prop = {};
			prop.m_familyIndex = m_graphicsQueueFamily.m_familyIndex;
			prop.m_flags = queueType;
			prop.m_queue = m_graphicsQueueFamily.GetFreeQueue();
			return MakeOwned<VQueue>(prop, this);
		}
		case VK_QUEUE_COMPUTE_BIT:
		{
			QueueProps prop = {};
			prop.m_familyIndex = m_computeQueueFamily.m_familyIndex;
			prop.m_flags = queueType;
			prop.m_queue = m_computeQueueFamily.GetFreeQueue();
			return MakeOwned<VQueue>(prop, this);
		}
		case VK_QUEUE_TRANSFER_BIT:
		{
			QueueProps prop = {};
			prop.m_familyIndex = m_transferQueueFamily.m_familyIndex;
			prop.m_flags = queueType;
			prop.m_queue = m_transferQueueFamily.GetFreeQueue();
			return MakeOwned<VQueue>(prop, this);
		}
		default:
			throw std::runtime_error("Queue type not supported!");
		}
	}

	Owned<VSemaphore >VDevice::CreateSyncSemaphore()
	{
		return MakeOwned<VSemaphore>(this);
	}

	Owned<VFence >VDevice::CreateSyncFence(bool signaled)
	{
		return MakeOwned<VFence>(signaled, this);
	}

	u32 VDevice::FindMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties) const
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

		ErrorUtils::LogAssert(false, "VDevice", "Failed to find suitable memory type!");
		return 0;
	}

	void VDevice::WaitForIdle() const
	{
		vkDeviceWaitIdle(m_device);
	}

	void VDevice::WaitForFence(VFence* fence) const
	{
		VkFence id = fence->GetFence();
		ErrorUtils::VkAssert(vkWaitForFences(m_device, 1, &id, VK_TRUE, u64_max), "VDevice");
	}

	void VDevice::ResetFence(VFence* fence) const
	{
		VkFence id = fence->GetFence();
		ErrorUtils::VkAssert(vkResetFences(m_device, 1, &id), "VDevice");
	}

	void VDevice::SubmitQueue(VQueue* queue, VCmdBuffer* cmdBuffer, VSemaphore* waitSem, VSemaphore* signalSem, VFence* fence, VkPipelineStageFlags flag) const
	{
		VkSemaphore wait = waitSem ? waitSem->GetSemaphore() : VK_NULL_HANDLE;
		VkSemaphore signal = signalSem ? signalSem->GetSemaphore() : VK_NULL_HANDLE;
		VkCommandBuffer cmd = cmdBuffer->GetCmdBuffer();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = waitSem ? 1 : 0;
		submitInfo.pWaitSemaphores = &wait;
		submitInfo.pWaitDstStageMask = &flag;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmd;
		submitInfo.signalSemaphoreCount = signalSem ? 1 : 0;
		submitInfo.pSignalSemaphores = &signal;

		ErrorUtils::VkAssert(vkQueueSubmit(queue->GetQueue(), 1, &submitInfo, fence ? fence->GetFence() : VK_NULL_HANDLE), "VDevice");
	}

	void VDevice::Destroy()
	{
		if (m_device != VK_NULL_HANDLE)
		{
			vkDestroyDevice(m_device, nullptr);
			m_device = VK_NULL_HANDLE;
		}
	}
}