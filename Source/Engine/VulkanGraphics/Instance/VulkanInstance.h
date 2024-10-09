/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include <vulkan/vulkan.h>

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

		VkInstance GetInstance() const { return m_instance; }
		VkPhysicalDevice GetAdapter() const { return m_adapter; }

		const String& GetAppName() const { return m_props.AppName; }
		const String& GetEngineName() const { return m_props.EngineName; }
		const Math::Vec3i& GetAppVersion() const { return m_props.AppVersion; }
		const Math::Vec3i& GetEngineVersion() const { return m_props.EngineVersion; }

	private:
		InstanceProps m_props;

		VkInstance m_instance;
		VkPhysicalDevice m_adapter;

#if defined(DELUSION_DEBUG)
		VkDebugUtilsMessengerEXT m_debugMessenger;
#endif // DELUSION_DEBUG
	};
}
