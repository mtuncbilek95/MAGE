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

	struct CmdBufferProps final
	{
		VkCommandBufferLevel m_cmdLevel;
		VCmdPool* m_pool;
	};

	class VCmdBuffer final : public VObject
	{
	public:
		VCmdBuffer(const CmdBufferProps& desc, VDevice* device);
		~VCmdBuffer();

		inline VkCommandBuffer GetCmdBuffer() const { return m_cmdBuffer; }

		void BeginRecording(VkCommandBufferUsageFlags useFlag) const;
		void BeginRecording(VRenderPass* renderPass, VFramebuffer* framebuffer) const;
		void EndRecording() const;

		void BeginRenderPass(VRenderPass* renderPass, VFramebuffer* framebuffer, VkSubpassContents passFlag) const;
		void EndRenderPass() const;

		void DrawIndexed(u32 indexCount, u32 indexOffset, u32 vertexOffset, u32 instanceOffset, u32 instanceCount) const;
		void ExecuteCommand(VCmdBuffer* buffer) const;

		void BindPipeline(VPipeline* pipeline) const;
		void BindDynamicState(VkViewport viewport, VkRect2D scissor) const;
		void BindVertexBuffer(VDstBuffer* vertexBuffer) const;
		void BindIndexBuffer(VDstBuffer* indexBuffer, u32 offset) const;
		void BindDescriptorBuffer(VDescBuffer* dscBuffer) const;

		void CopyBufferToBuffer(VStageBuffer* srcBuffer, VDstBuffer* dstBuffer) const;
		void CopyImageToBuffer(VStageBuffer* srcBuffer, VDstBuffer* dstBuffer) const;
		void Destroy() override final;

	private:
		VkCommandBuffer m_cmdBuffer;

		CmdBufferProps m_props;
	};
}
