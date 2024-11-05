/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Objects/IObject.h"
#include <vulkan/vulkan.hpp>

namespace MAGE
{
	class VInstance;
	class VQueue;

	struct DeviceProps final
	{
		DeviceProps(u32 gCount = 1, u32 tCount = 1, u32 cCount = 1) : graphicsQueueCount(gCount), 
			transferQueueCount(tCount), computeQueueCount(cCount) {}

		u32 graphicsQueueCount;
		u32 transferQueueCount;
		u32 computeQueueCount;
	};

	class VDevice final : public IObject
	{
		struct QueueFamily
		{
			QueueFamily() : m_familyIndex(255), m_queueCount(0), m_requestedCount(0) {};
			~QueueFamily() = default;

			vk::Queue GetFreeQueue()
			{
				if (m_queueCount > 0)
				{
					vk::Queue queue = m_queues.back();
					m_queues.pop_back();
					m_queueCount--;
					return queue;
				}

				throw std::runtime_error("Don't push yourself too hard over something we don't have.");
			}

			void FillQueues(vk::Device device)
			{
				for (u32 i = 0; i < m_requestedCount; i++)
				{
					vk::Queue queue = device.getQueue(m_familyIndex, i);
					m_queues.push_back(queue);
				}
			}

			u32 m_requestedCount;
			u32 m_familyIndex;
			u32 m_queueCount;
			Vector<vk::Queue> m_queues;
		};
	public:
		VDevice(const DeviceProps& desc, VInstance* instance);
		~VDevice() override final;
		
		inline vk::Device GetVkDevice() const { return m_device; }
		inline vk::Instance GetVkInstance() const { return m_instance; }
		inline vk::PhysicalDevice GetVkAdapter() const { return m_adapter; }

		u32 FindMemoryType(u32 typeFilter, vk::MemoryPropertyFlagBits properties);

		Owned<VQueue> CreateQueue(vk::QueueFlagBits queueType);

		void Destroy() override final;
		
	private:
		vk::Device m_device;
		vk::Instance m_instance;
		vk::PhysicalDevice m_adapter;
		vk::DispatchLoaderDynamic m_loader;

		QueueFamily m_graphicsQueueFamily;
		QueueFamily m_computeQueueFamily;
		QueueFamily m_transferQueueFamily;
	};
}
