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

#include <vulkan/vulkan.h>

namespace MAGE
{
	struct ShaderProps final
	{
		string shaderPath;
		string includePath = "";
		string entryPoint = "main";
		VkShaderStageFlagBits shaderStage;
	};

	class VShader final : public VkObject
	{
	public:
		VShader(const ShaderProps& desc, VDevice* device);
		~VShader() override final;

		inline VkShaderModule GetVkShaderModule() const { return m_shaderModule; }
		inline VkShaderStageFlagBits GetVkShaderStage() const { return m_props.shaderStage; }

		const string& GetShaderPath() const { return m_props.shaderPath; }
		const string& GetIncludePath() const { return m_props.includePath; }
		const string& GetEntryPoint() const { return m_props.entryPoint; }

		void Destroy() override final;

	private:
		ShaderProps m_props;

		VkShaderModule m_shaderModule;
	};
}