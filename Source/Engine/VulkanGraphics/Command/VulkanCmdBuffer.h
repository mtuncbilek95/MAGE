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
	class VulkanCmdPool;
	class VulkanDevice;

	struct CmdBufferProps final
	{
		VkCommandBufferLevel m_cmdLevel;
		VulkanCmdPool* m_pool;
	};

	class VulkanCmdBuffer final
	{
	public:
		VulkanCmdBuffer(const CmdBufferProps& desc, VulkanDevice* device);
		~VulkanCmdBuffer();

		VkCommandBuffer GetCmdBuffer() const { return m_cmdBuffer; }
		VkCommandPool GetCmdPool() const { return m_cmdPool; }

	private:
		VkQueue m_queue;
		VkCommandBuffer m_cmdBuffer;
		VkCommandPool m_cmdPool;
		VkDevice m_device;

		CmdBufferProps m_props;
	};
}
