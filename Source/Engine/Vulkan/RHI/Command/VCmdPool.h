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
	class VQueue;
	class VCmdBuffer;

	struct CmdPoolProps final
	{
		VQueue* queue = nullptr;
		vk::CommandPoolCreateFlags createFlags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
	};

	class VCmdPool final : public VkObject
	{
	public:
		VCmdPool(const CmdPoolProps& desc, VDevice* device);
		~VCmdPool() override final;

		Owned<VCmdBuffer> CreateBuffer(vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary);

		inline vk::CommandPool GetVkCmdPool() const { return m_pool; }

		void Destroy() override final;

	private:
		CmdPoolProps m_props;

		vk::CommandPool m_pool;
	};
}
