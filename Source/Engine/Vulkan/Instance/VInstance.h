/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Objects/IObject.h"
#include <vulkan/vulkan.h>

namespace MAGE
{
	struct InstanceProps final
	{
		string appName;
		string engineName;
		Math::Vec3i appVersion;
		Math::Vec3i engineVersion;
	};

	class VInstance final : public IObject
	{
	public:
		VInstance(const InstanceProps& desc);
		~VInstance() override final;

		inline VkInstance GetVkInstance() const { return m_instance; }
		inline VkPhysicalDevice GetVkAdapter() const { return m_adapter; }

		const string& GetAppName() const { return m_props.appName; }
		const string& GetEngineName() const { return m_props.engineName; }
		const Math::Vec3i& GetAppVersion() const { return m_props.appVersion; }
		const Math::Vec3i& GetEngineVersion() const { return m_props.engineVersion; }

		void Destroy() override final;

	private:
		InstanceProps m_props; // Instance properties

		VkInstance m_instance;
		VkPhysicalDevice m_adapter;

#if defined(DELUSION_DEBUG)
		VkDebugUtilsMessengerEXT m_debugMessenger;
#endif // DELUSION_DEBUG
	};
}
