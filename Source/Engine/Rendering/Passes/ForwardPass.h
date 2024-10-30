/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"

#include "Engine/VulkanRHI/Shader/VShader.h"
#include "Engine/VulkanRHI/Command/VCmdPool.h"
#include "Engine/VulkanRHI/Command/VCmdBuffer.h"
#include "Engine/VulkanRHI/Pipeline/VPipeline.h"
#include "Engine/VulkanRHI/RenderPass/VRenderPass.h"
#include "Engine/VulkanRHI/Framebuffer/VFramebuffer.h"

namespace MAGE
{
	class ForwardPass final
	{
	public:
		ForwardPass();
		~ForwardPass();

		VRenderPass* GetRenderPass() const { return &*m_renderPass; }
		VFramebuffer* GetFramebuffer(u32 index) const { return &*m_framebuffers[index]; }
		VPipeline* GetPipeline() const { return &*m_pipeline; }

	private:
		Owned<VRenderPass> m_renderPass;
		Vector<Owned<VFramebuffer>> m_framebuffers;

		Owned<VPipeline> m_pipeline;

		Owned<VShader> m_vShader;
		Owned<VShader> m_fShader;
	};
}