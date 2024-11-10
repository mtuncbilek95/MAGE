#include "VShader.h"

#include "../Device/VDevice.h"
#include "Engine/Vulkan/Core/VkAssert.h"

#include "Engine/ShaderCompiler/ShaderCompiler.h"
#include "Engine/Data/Memory/OwnedBuffer.h"

namespace MAGE
{
	VShader::VShader(const ShaderProps& desc, VDevice* device) : VkObject(device), m_props(desc), m_shaderModule(VK_NULL_HANDLE)
	{
		OwnedBuffer data = ShaderCompiler::CompileShader(desc.shaderPath, desc.includePath, desc.entryPoint);

		vk::ShaderModuleCreateInfo createInfo{};
		createInfo.codeSize = static_cast<u32>(data.Size());
		createInfo.pCode = reinterpret_cast<const u32*>(data.Data());

		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().createShaderModule(&createInfo, nullptr, &m_shaderModule), "VulkanShader");
	}

	VShader::~VShader()
	{
		Destroy();
	}

	void MAGE::VShader::Destroy()
	{
		if (m_shaderModule != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().destroyShaderModule(m_shaderModule);
			m_shaderModule = VK_NULL_HANDLE;
		}
	}
}
