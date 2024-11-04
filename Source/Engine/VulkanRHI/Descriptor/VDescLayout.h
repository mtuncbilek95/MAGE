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
	class VDescBuffer;

	struct DescLayoutBinding final
	{
		u32 binding;
		VkDescriptorType type;
		u32 count;
		VkShaderStageFlags stageFlags;
	};

	struct DescLayoutProps final
	{
		Vector<DescLayoutBinding> bindings;
		VkDescriptorSetLayoutCreateFlags initFlags;
	};

	class VDescLayout final : public VObject
	{
	public:
		VDescLayout(const DescLayoutProps& desc, VDevice* device);
		~VDescLayout() override final;

		Owned<VDescBuffer> CreateDescBuffer(u32 bufferCount, VkBufferUsageFlags flags);

		inline VkDescriptorSetLayout GetLayout() const { return m_layout; }
		inline VkDescriptorSetLayoutCreateFlags GetFlags() const { return m_props.initFlags; }

		void Destroy() override final;

	private:
		DescLayoutProps m_props;

		VkDescriptorSetLayout m_layout;
	};
}
