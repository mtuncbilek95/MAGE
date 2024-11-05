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
	struct QueueProps final
	{
		QueueProps(u32 index = 255, vk::QueueFlagBits flag = vk::QueueFlagBits::eGraphics) : 
			familyIndex(index), flags(flag) {}

		u32 familyIndex;
		vk::QueueFlagBits flags;
	};

	class VQueue final : public VkObject
	{
	public:
		VQueue(const QueueProps& desc, vk::Queue queue, VDevice* device);
		~VQueue() override final;

		inline vk::Queue GetVkQueue() const { return m_queue; }

		void Destroy() override final;

	private:
		vk::Queue m_queue;

		QueueProps m_props;
	};
}
