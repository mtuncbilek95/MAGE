#include "VRenderPass.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Device/VDevice.h"

namespace MAGE
{
	VRenderPass::VRenderPass(const RenderPassProps& desc, VDevice* device) : VObject(device), m_props(desc), m_renderPass(VK_NULL_HANDLE)
	{
		Vector<VkAttachmentDescription> attachments; // all attachments
		
		VkSubpassDependency subDependency = {}; // dependency
		subDependency.srcSubpass = VK_SUBPASS_EXTERNAL;

		if (desc.hasColor)
		{
			VkAttachmentDescription attDesc = {};
			attDesc.initialLayout = desc.colorPass.initialOut;
			attDesc.finalLayout = desc.colorPass.finalOut;
			attDesc.format = desc.colorPass.format;
			attDesc.samples = VK_SAMPLE_COUNT_1_BIT;
			attDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attDesc.flags = VkAttachmentDescriptionFlags();
			attachments.push_back(attDesc);

			subDependency.srcStageMask |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			subDependency.dstStageMask |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			subDependency.srcAccessMask |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			subDependency.dstAccessMask |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
		}

		if (desc.hasDepth)
		{
			VkAttachmentDescription attDesc = {};
			attDesc.initialLayout = desc.depthPass.initialOut;
			attDesc.finalLayout = desc.depthPass.finalOut;
			attDesc.format = desc.depthPass.format;
			attDesc.samples = VK_SAMPLE_COUNT_1_BIT;
			attDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attDesc.flags = VkAttachmentDescriptionFlags();
			attachments.push_back(attDesc);

			subDependency.srcStageMask |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			subDependency.dstStageMask |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			subDependency.srcAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			subDependency.dstAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
		}

		VkAttachmentReference colorRef; // color attachment reference
		colorRef.attachment = 0;
		colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthRef; // depth attachment reference
		depthRef.attachment = desc.hasColor ? 1 : 0;
		depthRef.layout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subPass = {}; // subPass
		subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subPass.colorAttachmentCount = desc.hasColor ? 1 : 0;
		subPass.pColorAttachments = desc.hasColor ? &colorRef : nullptr;
		subPass.pDepthStencilAttachment = desc.hasDepth ? &depthRef : nullptr;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<u32>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subPass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &subDependency;
		renderPassInfo.flags = VkRenderPassCreateFlags();

		ErrorUtils::VkAssert(vkCreateRenderPass(m_rootDevice->GetDevice(), &renderPassInfo, nullptr, &m_renderPass), "VRenderPass");
	}

	VRenderPass::~VRenderPass()
	{
		Destroy();
	}

	void VRenderPass::Destroy()
	{
		if (m_renderPass != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(m_rootDevice->GetDevice(), m_renderPass, nullptr);
			m_renderPass = VK_NULL_HANDLE;
		}
	}
}
