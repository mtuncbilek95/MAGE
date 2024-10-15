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
		VulkanQueue* queue;
		VkCommandPoolCreateFlags flags;
	};

	class VulkanCmdPool final : public std::enable_shared_from_this<VulkanCmdPool>
	{
	public:
		VulkanCmdPool(const CmdPoolProps& desc, VulkanDevice* device);
		~VulkanCmdPool();

		Owned<VulkanCmdBuffer> CreateCmdBuffer(VkCommandBufferLevel cmdLevel);

		VkCommandPool GetCmdPool() const { return m_cmdPool; }
		VulkanQueue* GetQueue() const { return m_props.queue; }

	private:
		CmdPoolProps m_props;
		VulkanDevice* m_deviceRef;

		VkCommandPool m_cmdPool;
	};
}
