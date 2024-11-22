/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Vulkan/Core/VkObject.h"

#include <vulkan/vulkan.h>

namespace MAGE
{
	class VCmdPool;
	class VPipeline;
	class VImageView;
	class VImage;
	class VBuffer;
	class VDescSet;

	struct CmdBufferProps final
	{
		VkCommandBufferLevel m_cmdLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		VCmdPool* m_pool = nullptr;
	};

	struct InheritanceProps final
	{
		vector<VkFormat> colorAttachments;
		VkFormat depthAttachment = VK_FORMAT_UNDEFINED;
		VkFormat stencilAttachment = VK_FORMAT_UNDEFINED;
		VkRenderingFlags renderFlags = VK_RENDERING_CONTENTS_SECONDARY_COMMAND_BUFFERS_BIT;
		VkSampleCountFlagBits msaaCount = VK_SAMPLE_COUNT_1_BIT;
		u32 viewMask = 0;

		VkCommandBufferUsageFlags usageFlags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	};

	struct RenderAttachment final
	{
		VImageView* view = nullptr;
		VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
		VkAttachmentLoadOp loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		VkAttachmentStoreOp storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		VkClearColorValue color = { 0.1f, 0.2f, 0.3f, 1.0f };
		VkClearDepthStencilValue depthStencil = VkClearDepthStencilValue(1.0f, 0);
	};

	struct RenderProps final
	{
		vector<RenderAttachment> colorAttachment;
		b8 hasDepth = false;
		RenderAttachment depthAttachment;
		b8 hasStencil = false;
		RenderAttachment stencilAttachment;
		VkExtent2D extent;
		VkOffset2D offset = { 0, 0 };
		u32 layerCount = 1;
		u32 viewMask = 0;
		VkRenderingFlags renderFlags = VK_RENDERING_CONTENTS_SECONDARY_COMMAND_BUFFERS_BIT;
	};

	struct ImageBarrierProps final
	{
		VkPipelineStageFlags srcPipeline = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkPipelineStageFlags dstPipeline = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		VkAccessFlags srcAccessMask;
		VkAccessFlags dstAccessMask;
		VkImageLayout oldLayout;
		VkImageLayout newLayout;
		u32	srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		u32 dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		VImage* image;

		VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		u32 baseMipLevel = 0;
		u32 levelCount = 1;
		u32 baseArrayLayer = 0;
		u32 layerCount = 1;
	};

	class VCmdBuffer final : public VkObject
	{
	public:
		VCmdBuffer(const CmdBufferProps& desc, VDevice* device);
		~VCmdBuffer();

		inline VkCommandBuffer GetVkCmdBuffer() const { return m_buffer; }

		void BeginRecord(VkCommandBufferUsageFlags flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) const;
		void BeginRecord(const InheritanceProps& desc) const;
		void EndRecord() const;

		void BeginRendering(const RenderProps& desc) const;
		void EndRendering() const;

		void ImageBarrier(const ImageBarrierProps& desc) const;

		void BindPipeline(VPipeline* pipeline);
		void BindDescriptors(const vector<VDescSet*>& sets) const;
		void BindVertexBuffer(const vector<VBuffer*>& buffers) const;
		void BindIndexBuffer(VBuffer* iBuffer, usize offset) const;
		void DrawIndexed(u32 indexCount, u32 indexOffset, u32 vertexOffset, u32 instanceOffset, u32 instanceCount) const;

		void ExecuteCommands(const vector<VCmdBuffer*>& buffers) const;

		void Destroy() override final;

	private:
		VkCommandBuffer m_buffer;

		CmdBufferProps m_props;

		VPipeline* m_boundPipeline;
	};
}
