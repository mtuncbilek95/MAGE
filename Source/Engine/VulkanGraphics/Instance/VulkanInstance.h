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
	struct InstanceProps final
	{
		String appName;
		String engineName;
		Math::Vec3i appVersion;
		Math::Vec3i engineVersion;
	};

	class VulkanInstance final
	{
	public:
		VulkanInstance(const InstanceProps& desc);
		~VulkanInstance();

		VkInstance GetInstance() const { return m_instance; }
		VkPhysicalDevice GetAdapter() const { return m_adapter; }

		const String& GetAppName() const { return m_props.appName; }
		const String& GetEngineName() const { return m_props.engineName; }
		const Math::Vec3i& GetAppVersion() const { return m_props.appVersion; }
		const Math::Vec3i& GetEngineVersion() const { return m_props.engineVersion; }

	private:
		InstanceProps m_props; // Instance properties

		VkInstance m_instance;
		VkPhysicalDevice m_adapter;

#if defined(DELUSION_DEBUG)
		VkDebugUtilsMessengerEXT m_debugMessenger;
#endif // DELUSION_DEBUG
	};
}
