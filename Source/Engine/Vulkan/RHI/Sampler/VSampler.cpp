#include "VSampler.h"

#include "../Device/VDevice.h"
#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	VSampler::VSampler(const SamplerProps& desc, VDevice* device) : VkObject(device), m_props(desc)
	{
		vk::SamplerCreateInfo samplerInfo = vk::SamplerCreateInfo();
		samplerInfo.magFilter = desc.magFilter;
		samplerInfo.minFilter = desc.minFilter;
		samplerInfo.mipmapMode = desc.mipmapMode;
		samplerInfo.addressModeU = desc.addressModeU;
		samplerInfo.addressModeV = desc.addressModeV;
		samplerInfo.addressModeW = desc.addressModeW;
		samplerInfo.mipLodBias = desc.mipLodBias;
		samplerInfo.anisotropyEnable = desc.anisotropyEnable;
		samplerInfo.maxAnisotropy = desc.maxAnisotropy;
		samplerInfo.compareEnable = desc.compareEnable;
		samplerInfo.compareOp = desc.compareOp;
		samplerInfo.minLod = desc.minLod;
		samplerInfo.maxLod = desc.maxLod;

		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().createSampler(&samplerInfo, nullptr, &m_sampler), "VSampler");
	}

	VSampler::~VSampler()
	{
		Destroy();
	}

	void VSampler::Destroy()
	{
		if (m_sampler != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().destroySampler(m_sampler);
			m_sampler = VK_NULL_HANDLE;
		}
	}
}
