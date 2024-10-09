/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"

#include <vulkan/vulkan.hpp>

namespace MAGE
{
	struct ENGINE_API InstanceProps final
	{
		String AppName;
		String EngineName;
		Math::Vec3i AppVersion;
		Math::Vec3i EngineVersion;
	};

	class ENGINE_API VulkanInstance final
	{
	public:
		VulkanInstance(const InstanceProps& desc);
		~VulkanInstance();

	private:
		InstanceProps m_props;

		vk::Instance m_instance;
	};
}
