#include "VulkanShader.h"

#include "../Core/VkAssert.h"
#include "../Device/VulkanDevice.h"

#include "Engine/ShaderCompiler/ShaderCompiler.h"

namespace MAGE
{
	VulkanShader::VulkanShader(const ShaderProps& desc, VulkanDevice* device) : m_device(device->GetDevice()), 
		m_physicalDevice(device->GetAdapter()), m_props(desc)
	{
		// Compile shader first
		m_shaderData = ShaderCompiler::CompileShader(desc.shaderPath, desc.includePath, desc.entryPoint);

		// Load shader code from file
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = static_cast<u32>(m_shaderData.Size());
		createInfo.pCode = reinterpret_cast<const u32*>(m_shaderData.Data());

		ErrorUtils::VkAssert(vkCreateShaderModule(m_device, &createInfo, nullptr, &m_shaderModule));
	}

	VulkanShader::~VulkanShader()
	{
		vkDestroyShaderModule(m_device, m_shaderModule, nullptr);
	}
}
