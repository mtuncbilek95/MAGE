#include "VShader.h"
#include "VShaderUtils.h"

#include "Engine/Vulkan/Device/VDevice.h"
#include "Engine/Platform/PlatformErrorMessage.h"

namespace MAGE
{
	VShader::VShader(const ShaderDesc& desc, VDevice* pDevice) : Shader(desc, pDevice), mDevice(pDevice->GetVkDevice())
	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = desc.ShaderCode.Size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(desc.ShaderCode.Data());
		createInfo.flags = 0;

		MAGE_ASSERT(vkCreateShaderModule(mDevice, &createInfo, nullptr, &mShaderModule) == VK_SUCCESS, "VShader", "Failed to create shader module");
	}
}
