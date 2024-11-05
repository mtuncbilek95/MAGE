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
#include <vulkan/vulkan.hpp>

namespace MAGE
{
	struct InstanceProps final
	{
		InstanceProps(const String& aName = "MAGE", const String& eName = "MAGE", 
			const Math::Vec3i& aVersion = { 1, 0, 0 }, const Math::Vec3i& eVersion = { 1, 0, 0 }) : 
			appName(aName), engineName(eName), appVersion(aVersion), engineVersion(eVersion) {}

		String appName;
		String engineName;
		Math::Vec3i appVersion;
		Math::Vec3i engineVersion;
	};

	class VInstance final : public IObject
	{
	public:
		VInstance(const InstanceProps& desc);
		~VInstance() override final;

		inline vk::Instance GetVkInstance() const { return m_instance; }
		inline vk::PhysicalDevice GetVkAdapter() const { return m_adapter; }
		inline vk::DispatchLoaderDynamic GetVkLoader() const { return m_loader; }

		void Destroy() override final;

		const String& GetApiName() const { return m_props.appName; }
		const String& GetEngineName() const { return m_props.engineName; }
		const Math::Vec3i& GetApiVersion() const { return m_props.appVersion; }
		const Math::Vec3i& GetEngineVersion() const { return m_props.engineVersion; }

	private:
		vk::Instance m_instance;
		vk::PhysicalDevice m_adapter;
		vk::DispatchLoaderDynamic m_loader;

		InstanceProps m_props;

#if defined(DELUSION_DEBUG)
		vk::DebugUtilsMessengerEXT m_debugger;
#endif
	};
}
