#include "VShader.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Device/VDevice.h"

#include "Engine/ShaderCompiler/ShaderCompiler.h"
#include "Engine/Memory/OwnedBuffer.h"

namespace MAGE
{
	VShader::VShader(const ShaderProps& desc, VDevice* device) : VObject(device), m_props(desc), m_shaderModule(VK_NULL_HANDLE)
	{
		// Compile shader first
		OwnedBuffer data = ShaderCompiler::CompileShader(desc.shaderPath, desc.includePath, desc.entryPoint);

		// Load shader code from file
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = static_cast<u32>(data.Size());
		createInfo.pCode = reinterpret_cast<const u32*>(data.Data());

		ErrorUtils::VkAssert(vkCreateShaderModule(m_rootDevice->GetDevice(), &createInfo, nullptr, &m_shaderModule), "VulkanShader");
	}

	VShader::~VShader()
	{
		Destroy();
	}

	void MAGE::VShader::Destroy()
	{
		if (m_shaderModule != VK_NULL_HANDLE)
		{
			vkDestroyShaderModule(m_rootDevice->GetDevice(), m_shaderModule, nullptr);
			m_shaderModule = VK_NULL_HANDLE;
		}
	}
}
