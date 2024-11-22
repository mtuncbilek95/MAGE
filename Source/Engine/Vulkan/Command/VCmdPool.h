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
	class VQueue;
	class VCmdBuffer;

	struct CmdPoolProps final
	{
		VQueue* queue = nullptr;
		VkCommandPoolCreateFlags flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	};

	class VCmdPool final : public VkObject
	{
	public:
		VCmdPool(const CmdPoolProps& desc, VDevice* device);
		~VCmdPool() override final;

		inline VkCommandPool GetVkCmdPool() const { return m_cmdPool; }
		VQueue* GetVkQueue() const { return m_props.queue; }

		Owned<VCmdBuffer> CreateCmdBuffer(VkCommandBufferLevel cmdLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		void Destroy() override final;

	private:
		CmdPoolProps m_props;

		VkCommandPool m_cmdPool;
	};
}
