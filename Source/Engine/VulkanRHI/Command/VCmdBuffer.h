/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "../Abstraction/VObject.h"

namespace MAGE
{
	class VCmdPool;
	class VRenderPass;
	class VFramebuffer;
	class VPipeline;
	class VDstBuffer;
	class VDescBuffer;
	class VStageBuffer;
	class VImage;
	class VImageView;

	struct CmdBufferProps final
	{
		VkCommandBufferLevel m_cmdLevel;
		VCmdPool* m_pool;
	};

	struct RecordingProps final
	{
		b8 onlyTransfer = false;

		u32 subPass = 0;
		Vector<VkFormat> colorAttachments;
		VkFormat depthAttachment = VK_FORMAT_UNDEFINED;
		VkFormat stencilAttachment = VK_FORMAT_UNDEFINED;
		VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT;
		u32 viewMask;

		VkCommandBufferUsageFlags beginFlag = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
	};

	struct RenderAttachment final
	{
		VImageView* view = nullptr;
		VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
		VkAttachmentLoadOp loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		VkAttachmentStoreOp storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		Math::Vec4f colorValue = { 0.1f, 0.2f, 0.3f, 1.0f };
		Math::Vec2f depthValue = { 1.0f, 0.0f };
	};

	struct RenderingProps final
	{
		Vector<RenderAttachment> colorAttachments;
		b8 hasDepth = false;
		RenderAttachment depthAttachment;
		b8 hasStencil = false;
		RenderAttachment stencilAttachment;

		Math::Vec2u extent;
		Math::Vec2i offset;
		u32 viewMask;
		u32 layerCount;
	};

	struct ImageBarrierProps final
	{
		VkPipelineStageFlags srcPipeline;
		VkPipelineStageFlags dstPipeline;

		VkAccessFlags srcAccessMask;
		VkAccessFlags dstAccessMask;
		VkImageLayout oldLayout;
		VkImageLayout newLayout;
		u32	srcQueueFamilyIndex;
		u32 dstQueueFamilyIndex;
		VImage* image;

		VkImageAspectFlags aspectMask;
		u32 baseMipLevel;
		u32 levelCount;
		u32 baseArrayLayer;
		u32 layerCount;
	};

	class VCmdBuffer final : public VObject
	{
	public:
		VCmdBuffer(const CmdBufferProps& desc, VDevice* device);
		~VCmdBuffer();

		inline VkCommandBuffer GetCmdBuffer() const { return m_cmdBuffer; }

		void BeginRecording(VkCommandBufferUsageFlags useFlag) const;
		void BeginRecording(const RecordingProps& recordProp) const;
		void EndRecording() const;

		void BeginRenderPass(VRenderPass* renderPass, VFramebuffer* framebuffer, VkSubpassContents passFlag) const;
		void EndRenderPass() const;

		void BeginRendering(const RenderingProps& renderProp) const;
		void EndRendering();

		void DrawIndexed(u32 indexCount, u32 indexOffset, u32 vertexOffset, u32 instanceOffset, u32 instanceCount) const;

		void ExecuteCommand(VCmdBuffer* buffer) const;
		void KillThemAll(const Vector<VCmdBuffer*>& buffers) const;

		void BindPipeline(VPipeline* pipeline) const;
		void BindDynamicState(VkViewport viewport, VkRect2D scissor) const;
		void BindVertexBuffer(VDstBuffer* vertexBuffer) const;
		void BindIndexBuffer(VDstBuffer* indexBuffer, u32 offset) const;
		void BindDescriptorBuffer(VDescBuffer* dscBuffer) const;

		void CopyBufferToBuffer(VStageBuffer* srcBuffer, VDstBuffer* dstBuffer) const;
		void CopyBufferToImage(VStageBuffer* srcBuffer, VImage* dstBuffer) const;

		void TranslateImageBarrier(const ImageBarrierProps& barrierInfo) const;

		void Destroy() override final;

	private:
		VkCommandBuffer m_cmdBuffer;

		CmdBufferProps m_props;

		VRenderPass* boundPass;
		VFramebuffer* boundFrame;
	};
}
