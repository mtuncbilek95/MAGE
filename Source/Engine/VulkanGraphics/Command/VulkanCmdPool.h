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
	class VulkanQueue;
	class VulkanCmdBuffer;

	struct CmdPoolProps final
	{
		VulkanQueue* m_queue;
		VkCommandPoolCreateFlags m_flags;
	};

	class VulkanCmdPool final
	{
	public:
		VulkanCmdPool(const CmdPoolProps& desc, VulkanDevice* device);
		~VulkanCmdPool();

		Shared<VulkanCmdBuffer> CreateCmdBuffer(VkCommandBufferLevel cmdLevel);

		VkCommandPool GetCmdPool() const { return m_cmdPool; }
		VkDevice GetDevice() const { return m_device; }

	private:
		VkCommandPool m_cmdPool;
		VkDevice m_device;

		VulkanDevice* m_deviceRef;
	};
}
