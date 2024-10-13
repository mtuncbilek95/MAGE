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

#include "Engine/Memory/OwnedBuffer.h"

namespace MAGE
{
	class VulkanDevice;

	struct ShaderProps final
	{
		String shaderPath;
		String includePath = "";
		String entryPoint = "main";
		VkShaderStageFlagBits shaderStage;
	};

	class VulkanShader final
	{
	public:
		VulkanShader(const ShaderProps& desc, VulkanDevice* device);
		~VulkanShader();

		VkShaderModule GetShaderModule() const { return m_shaderModule; }
		
		const String& GetShaderPath() const { return m_props.shaderPath; }
		const String& GetIncludePath() const { return m_props.includePath; }
		const String& GetEntryPoint() const { return m_props.entryPoint; }
		VkShaderStageFlagBits GetShaderStage() const { return m_props.shaderStage; }

	private:
		VkShaderModule m_shaderModule;

		ShaderProps m_props;

		VkDevice m_device;
		VkPhysicalDevice m_physicalDevice;

		OwnedBuffer m_shaderData;
	};
}