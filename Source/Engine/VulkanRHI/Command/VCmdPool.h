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
	class VQueue;
	class VCmdBuffer;

	struct CmdPoolProps final
	{
		VQueue* queue;
		VkCommandPoolCreateFlags flags;
	};

	class VCmdPool final : public VObject
	{
	public:
		VCmdPool(const CmdPoolProps& desc, VDevice* device);
		~VCmdPool() override final;

		inline VkCommandPool GetCmdPool() const { return m_cmdPool; }
		VQueue* GetQueue() const { return m_props.queue; }

		Owned<VCmdBuffer> CreateCmdBuffer(VkCommandBufferLevel cmdLevel);
		void Destroy() override final;

	private:
		CmdPoolProps m_props;

		VkCommandPool m_cmdPool;
	};
}
