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

#include "Engine/VulkanGraphics/Core/VulkanImageBarrier.h"

namespace MAGE
{
	class VulkanImageView;
	class VulkanCmdPool;
	class VulkanDevice;
	class VulkanRenderPass;

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

		void BeginRecording(VkCommandBufferUsageFlags useFlag) const;
		void EndRecording() const;

		void BeginRenderPass(VulkanImageView* view, Math::Vec2u renderArea) const;
		void EndRenderPass() const;

		void SetPipelineImageBarrier(const VulkanImageBarrier& barrier, VkPipelineStageFlagBits srcFlag, VkPipelineStageFlagBits dstFlag) const;

	private:
		VkCommandBuffer m_cmdBuffer;

		CmdBufferProps m_props;
		VulkanDevice* m_deviceRef;
	};
}
