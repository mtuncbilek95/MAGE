/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include <vulkan/vulkan.h>

namespace MAGE
{
	class VulkanInstance;
	class VulkanQueue;

	struct DeviceProps final
	{
		u32 m_graphicsQueueCount;
		u32 m_computeQueueCount;
		u32 m_transferQueueCount;
	};

	class VulkanDevice final
	{
		struct QueueFamily
		{
			QueueFamily() : m_familyIndex(255), m_queueCount(0), m_requestedCount(0) {};
			~QueueFamily() = default;

			VkQueue GetFreeQueue()
			{
				if (m_queueCount > 0)
				{
					VkQueue queue = m_queues.back();
					m_queues.pop_back();
					m_queueCount--;
					return queue;
				}

				return VK_NULL_HANDLE;
			}

			void FillQueues(VkDevice device)
			{
				for (u32 i = 0; i < m_requestedCount; i++)
				{
					VkQueue queue;
					vkGetDeviceQueue(device, m_familyIndex, i, &queue);
					m_queues.push_back(queue);
				}
			}

			u32 m_familyIndex;
			u32 m_queueCount;
			u32 m_requestedCount;
			Vector<VkQueue> m_queues;
		};

	public:
		VulkanDevice(const DeviceProps& desc, VulkanInstance* instance);
		~VulkanDevice();

		VkDevice GetDevice() const { return m_device; }
		VkPhysicalDevice GetAdapter() const { return m_adapter; }
		VkInstance GetInstance() const { return m_instance; }

		Owned<VulkanQueue> CreateQueue(VkQueueFlags queueType);

		u32 FindMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties);

	private:
		DeviceProps m_props;

		VkInstance m_instance;
		VkDevice m_device;
		VkPhysicalDevice m_adapter;

		QueueFamily m_graphicsQueueFamily;
		QueueFamily m_computeQueueFamily;
		QueueFamily m_transferQueueFamily;
	};
}
