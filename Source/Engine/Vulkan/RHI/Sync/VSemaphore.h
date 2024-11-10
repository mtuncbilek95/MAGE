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
	class VSemaphore final : public VkObject
	{
	public:
		VSemaphore(VDevice* device);
		~VSemaphore() override final;

		inline vk::Semaphore GetVkSemaphore() const { return m_semaphore; }

		void Destroy() override final;

	private:
		vk::Semaphore m_semaphore;
	};
}
