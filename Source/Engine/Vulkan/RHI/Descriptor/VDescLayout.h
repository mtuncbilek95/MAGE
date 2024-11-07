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
	class VDescBuffer;

	struct BindingProps final
	{
		u32 binding;
		vk::DescriptorType type;
		u32 count;
		vk::ShaderStageFlags stageFlags;
	};

	struct DescLayoutProps final
	{
		Vector<BindingProps> bindings;
		vk::DescriptorSetLayoutCreateFlags createFlags;
	};

	class VDescLayout final : public VkObject
	{
	public:
		VDescLayout(const DescLayoutProps& desc, VDevice* device);
		~VDescLayout() override final;

		Owned<VDescBuffer> CreateDescBuffer(u32 resourceCount, vk::BufferUsageFlags flags);

		inline vk::DescriptorSetLayout GetVkLayout() const { return m_layout; }
		vk::PhysicalDeviceDescriptorBufferPropertiesEXT GetBufferProps() const;

		void Destroy() override final;

	private:
		vk::DescriptorSetLayout m_layout;
	};
}
