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
	class VFence final : public VkObject
	{
	public:
		VFence(VDevice* device);
		~VFence() override final;

		inline vk::Fence GetVkFence() const { return m_fence; }

		void Wait() const;
		void Reset() const;

		void Destroy() override final;

	private:
		vk::Fence m_fence;
	};
}
