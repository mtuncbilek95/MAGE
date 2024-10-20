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
		void EndRecording() const;

		void BeginRenderPass(VRenderPass* renderPass, VFramebuffer* framebuffer) const;
		void EndRenderPass() const;

		void Destroy() override final;

	private:
		VkCommandBuffer m_cmdBuffer;

		CmdBufferProps m_props;
	};
}
