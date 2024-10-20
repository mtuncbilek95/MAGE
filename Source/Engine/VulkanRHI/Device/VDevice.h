/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "../Abstraction/VRoot.h"
#include "../Queue/VQueue.h"
#include "../Sync/VSemaphore.h"
#include "../Sync/VFence.h"
#include "../Command/VCmdBuffer.h"

namespace MAGE
{
	class VInstance;

	struct DeviceProps final
	{
		u32 graphicsQueueCount;
		u32 computeQueueCount;
		u32 transferQueueCount;
	};

	class VDevice final : public VRoot
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
		VDevice(const DeviceProps& desc, VInstance* instance);
		~VDevice() override final;

		inline VkDevice GetDevice() const { return m_device; }
		inline VkPhysicalDevice GetAdapter() const { return m_adapter; }
		inline VkInstance GetInstance() const { return m_instance; }

		Owned<VQueue> CreateQueue(VkQueueFlags queueType);
		Owned<VSemaphore> CreateSyncSemaphore();
		Owned<VFence> CreateSyncFence(bool signaled);

		u32 FindMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties) const;

		void WaitForIdle() const;
		void WaitForFence(VFence* fence) const;
		void ResetFence(VFence* fence) const;
		void SubmitQueue(VQueue* queue, VCmdBuffer* cmdBuffer, VSemaphore* waitSem, VSemaphore* signalSem, VFence* fence, VkPipelineStageFlags flag) const;

		void Destroy() override final;

	private:
		DeviceProps m_props; // Device properties

		VkInstance m_instance;
		VkDevice m_device;
		VkPhysicalDevice m_adapter;

		QueueFamily m_graphicsQueueFamily;
		QueueFamily m_computeQueueFamily;
		QueueFamily m_transferQueueFamily;
	};
}
