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
	struct DescPoolProps final
	{
		u32 maxSets;
		VkDescriptorPoolCreateFlags flags;
		Vector<VkDescriptorPoolSize> poolSizes;
	};

	class VDescPool final : public VObject
	{
	public:
		VDescPool(const DescPoolProps& desc, VDevice* device);
		~VDescPool() override final;

		inline VkDescriptorPool GetPool() const { return m_pool; }
		inline VkDescriptorPoolCreateFlags GetFlags() const { return m_props.flags; }

		void Destroy() override final;

	private:
		DescPoolProps m_props;

		VkDescriptorPool m_pool;
	};
}
