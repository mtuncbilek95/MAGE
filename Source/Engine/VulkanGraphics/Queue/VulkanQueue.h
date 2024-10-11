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
	class VulkanDevice;

	struct QueueProps final
	{
		u32 m_familyIndex;
		VkQueueFlags m_flags;
		VkQueue m_queue;
	};

	class VulkanQueue final
	{
	public:
		VulkanQueue(const QueueProps& desc, VulkanDevice* device);
		~VulkanQueue();

		VkQueue GetQueue() const { return m_props.m_queue; }
		u32 GetFamilyIndex() const { return m_props.m_familyIndex; }
		VkQueueFlags GetFlags() const { return m_props.m_flags; }

	private:
		QueueProps m_props;
		VkDevice m_device;
	};
}
