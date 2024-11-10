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

#include <vulkan/vulkan.hpp>

namespace MAGE
{
	class VCmdPool;
	class VPipeline;
	class VImageView;
	class VImage;

	struct CmdBufferProps final
	{
		VCmdPool* pool = nullptr;
		vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary;
	};

	struct InheritanceProps final
	{
		Vector<vk::Format> colorAttachments;
		vk::Format depthAttachment = vk::Format::eUndefined;
		vk::Format stencilAttachment = vk::Format::eUndefined;
		vk::RenderingFlags renderFlags = vk::RenderingFlagBits::eContentsSecondaryCommandBuffers;
		vk::SampleCountFlagBits msaaCount = vk::SampleCountFlagBits::e1;
		u32 viewMask = 0;

		vk::CommandBufferUsageFlags usageFlags = vk::CommandBufferUsageFlagBits::eRenderPassContinue;
	};

	struct RenderAttachment final
	{
		VImageView* view = nullptr;
		vk::ImageLayout layout = vk::ImageLayout::eUndefined;
		vk::AttachmentLoadOp loadOp = vk::AttachmentLoadOp::eLoad;
		vk::AttachmentStoreOp storeOp = vk::AttachmentStoreOp::eStore;
		vk::ClearColorValue color = vk::ClearColorValue(0.1f, 0.2f, 0.3f, 1.0f);
		vk::ClearDepthStencilValue depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
	};

	struct RenderProps final
	{
		Vector<RenderAttachment> colorAttachment;
		b8 hasDepth = false;
		RenderAttachment depthAttachment;
		b8 hasStencil = false;
		RenderAttachment stencilAttachment;
		vk::Extent2D extent;
		vk::Offset2D offset = { 0, 0 };
		u32 layerCount = 1;
		u32 viewMask = 0;
		vk::RenderingFlags renderFlags;
	};

	struct ImageBarrierProps final
	{
		vk::PipelineStageFlags srcPipeline;
		vk::PipelineStageFlags dstPipeline;

		vk::AccessFlags srcAccessMask;
		vk::AccessFlags dstAccessMask;
		vk::ImageLayout oldLayout;
		vk::ImageLayout newLayout;
		u32	srcQueueFamilyIndex = vk::QueueFamilyIgnored;
		u32 dstQueueFamilyIndex = vk::QueueFamilyIgnored;
		VImage* image;

		vk::ImageAspectFlags aspectMask = vk::ImageAspectFlagBits::eColor;
		u32 baseMipLevel = 0;
		u32 levelCount = 1;
		u32 baseArrayLayer = 0;
		u32 layerCount = 1;
	};

	class VCmdBuffer final : public VkObject
	{
	public:
		VCmdBuffer(const CmdBufferProps& desc, VDevice* device);
		~VCmdBuffer() override final;

		inline vk::CommandBuffer GetVkCmdBuffer() const { return m_buffer; }

		void BeginRecord(vk::CommandBufferUsageFlags flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit) const;
		void BeginRecord(const InheritanceProps& desc) const;
		void EndRecord() const;

		void BeginRendering(const RenderProps& desc) const;
		void EndRendering() const;

		void ImageBarrier(const ImageBarrierProps& desc) const;

		void Destroy() override final;

	private:
		CmdBufferProps m_props;

		vk::CommandBuffer m_buffer;
	};
}
