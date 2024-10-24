#include "VCmdBuffer.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Device/VDevice.h"
#include "../Command/VCmdPool.h"
#include "../Framebuffer/VFramebuffer.h"
#include "../RenderPass/VRenderPass.h"
#include "../Pipeline/VPipeline.h"
#include "../Buffer/VStageBuffer.h"
#include "../Buffer/VDstBuffer.h"

namespace MAGE
{
	VCmdBuffer::VCmdBuffer(const CmdBufferProps& desc, VDevice* device) : VObject(device), m_props(desc), m_cmdBuffer(VK_NULL_HANDLE)
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = desc.m_pool->GetCmdPool();
		allocInfo.level = desc.m_cmdLevel;
		allocInfo.commandBufferCount = 1;

		ErrorUtils::VkAssert(vkAllocateCommandBuffers(m_rootDevice->GetDevice(), &allocInfo, &m_cmdBuffer), "VCmdBuffer");
	}

	VCmdBuffer::~VCmdBuffer()
	{
		Destroy();
	}

	void VCmdBuffer::BeginRecording(VkCommandBufferUsageFlags useFlag) const
	{
		ErrorUtils::VkAssert(vkResetCommandBuffer(m_cmdBuffer, 0), "VCmdBuffer");

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = useFlag;
		beginInfo.pInheritanceInfo = nullptr;

		ErrorUtils::VkAssert(vkBeginCommandBuffer(m_cmdBuffer, &beginInfo), "VCmdBuffer");
	}

	void VCmdBuffer::BeginRecording(VRenderPass* renderPass, VFramebuffer* framebuffer) const
	{
		ErrorUtils::VkAssert(vkResetCommandBuffer(m_cmdBuffer, 0), "VCmdBuffer");

		VkCommandBufferInheritanceInfo inherit = {};
		inherit.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		inherit.renderPass = renderPass->GetRenderPass();
		inherit.framebuffer = framebuffer->GetBuffer();
		inherit.subpass = 0;
		inherit.occlusionQueryEnable = VK_FALSE;

		VkCommandBufferBeginInfo begin = {};
		begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		begin.pInheritanceInfo = &inherit;

		ErrorUtils::VkAssert(vkBeginCommandBuffer(m_cmdBuffer, &begin), "VCmdBuffer");
	}

	void VCmdBuffer::EndRecording() const
	{
		ErrorUtils::VkAssert(vkEndCommandBuffer(m_cmdBuffer), "VCmdBuffer");
	}

	void VCmdBuffer::BeginRenderPass(VRenderPass* renderPass, VFramebuffer* framebuffer, VkSubpassContents passFlag) const
	{
		VkRenderPassBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginInfo.renderPass = renderPass->GetRenderPass();
		beginInfo.framebuffer = framebuffer->GetBuffer();
		beginInfo.renderArea.extent = { framebuffer->GetRenderArea().x, framebuffer->GetRenderArea().y };
		beginInfo.renderArea.offset = { 0, 0 };
		beginInfo.clearValueCount = 1;

		VkClearValue colorVal = {};
		colorVal.color.float32[0] = 0.1f;
		colorVal.color.float32[1] = 0.2f;
		colorVal.color.float32[2] = 0.3f;
		colorVal.color.float32[3] = 1.0f;

		beginInfo.pClearValues = &colorVal;

		vkCmdBeginRenderPass(m_cmdBuffer, &beginInfo, passFlag);
	}

	void VCmdBuffer::EndRenderPass() const
	{
		vkCmdEndRenderPass(m_cmdBuffer);
	}

	void VCmdBuffer::DrawIndexed(u32 indexCount, u32 indexOffset, u32 vertexOffset, u32 instanceOffset, u32 instanceCount) const
	{
		vkCmdDrawIndexed(m_cmdBuffer, indexCount, instanceCount, 0, vertexOffset, 0);
	}

	void VCmdBuffer::ExecuteCommand(VCmdBuffer* buffer) const
	{
		VkCommandBuffer cmd = buffer->GetCmdBuffer();

		vkCmdExecuteCommands(m_cmdBuffer, 1, &cmd);
	}

	void VCmdBuffer::BindPipeline(VPipeline* pipeline) const
	{
		vkCmdBindPipeline(m_cmdBuffer, pipeline->GetBindPoint(), pipeline->GetPipeline());
	}

	void VCmdBuffer::BindDynamicState(VkViewport viewport, VkRect2D scissor) const
	{
	}

	void VCmdBuffer::BindVertexBuffer(VDstBuffer* vertexBuffer) const
	{
		u64 offset = 0;
		VkBuffer vBuffer = vertexBuffer->GetBuffer();
		vkCmdBindVertexBuffers(m_cmdBuffer, 0, 1, &vBuffer, &offset);
	}

	void VCmdBuffer::BindIndexBuffer(VDstBuffer* indexBuffer, u32 offset) const
	{
		vkCmdBindIndexBuffer(m_cmdBuffer, indexBuffer->GetBuffer(), offset, VK_INDEX_TYPE_UINT32);
	}

	void VCmdBuffer::BindDescriptorBuffer(VDescBuffer* dscBuffer) const
	{
	}

	void VCmdBuffer::CopyBufferToBuffer(VStageBuffer* srcBuffer, VDstBuffer* dstBuffer) const
	{
		VkBufferCopy copyInfo = {};
		copyInfo.dstOffset = 0;
		copyInfo.size = dstBuffer->GetTotalSize();
		copyInfo.srcOffset = 0;

		vkCmdCopyBuffer(m_cmdBuffer, srcBuffer->GetBuffer(), dstBuffer->GetBuffer(), 1, &copyInfo);
	}

	void VCmdBuffer::CopyImageToBuffer(VStageBuffer* srcBuffer, VDstBuffer* dstBuffer) const
	{
	}

	void VCmdBuffer::Destroy()
	{
		if (m_cmdBuffer != VK_NULL_HANDLE)
		{
			vkFreeCommandBuffers(m_rootDevice->GetDevice(), m_props.m_pool->GetCmdPool(), 1, &m_cmdBuffer);
			m_cmdBuffer = VK_NULL_HANDLE;
		}
	}
}
