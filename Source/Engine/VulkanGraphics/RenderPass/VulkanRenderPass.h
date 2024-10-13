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
	class VulkanImageView;

	struct RenderPassProps final
	{
		Math::Vec4f clearColor;
		f32 clearDepth;
	};

	/**
	 * @class VulkanRendering
	 * @brief Dynamic rendering class for Vulkan. It is responsible for 
	 * sending VkRenderingAttachmentInfo to vkCmdBeginRendering.
	 */
	class VulkanRenderPass final
	{
	public:
		VulkanRenderPass(const RenderPassProps& desc, VulkanDevice* device);
		~VulkanRenderPass();

		void AddAttachment(VulkanImageView* imageView);
		void RemoveAttachment(VulkanImageView* imageView);

		VulkanImageView* GetColorAttachment(u32 index) const { return m_colorAttachments[index]; }
		VulkanImageView* GetDepthAttachment() const { return m_depthAttachment; }

		Math::Vec4f GetClearColor() const { return m_props.clearColor; }
		f32 GetClearDepth() const { return m_props.clearDepth; }

	private:
		VulkanDevice* m_deviceRef;

		Vector<VulkanImageView*> m_colorAttachments;
		VulkanImageView* m_depthAttachment;

		RenderPassProps m_props;
	};
}
