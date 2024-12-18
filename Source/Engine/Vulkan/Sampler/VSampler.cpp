#include "VSampler.h"

#include "../Core/VkAssert.h"
#include "../Device/VDevice.h"

namespace MAGE
{
	VSampler::VSampler(const SamplerProps& desc, VDevice* device) : VkObject(device), m_props(desc)
	{
		VkSamplerCreateInfo info = { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
		info.magFilter = desc.magFilter;
		info.minFilter = desc.minFilter;
		info.mipmapMode = desc.mipmapMode;
		info.addressModeU = desc.addressModeU;
		info.addressModeV = desc.addressModeV;
		info.addressModeW = desc.addressModeW;
		info.mipLodBias = desc.mipLodBias;
		info.anisotropyEnable = desc.anisotropyEnable ? VK_TRUE : VK_FALSE;
		info.maxAnisotropy = desc.maxAnisotropy;
		info.compareEnable = desc.compareEnable ? VK_TRUE : VK_FALSE;
		info.compareOp = desc.compareOp;
		info.minLod = desc.minLod;
		info.maxLod = desc.maxLod;
		info.borderColor = desc.borderColor;
		info.unnormalizedCoordinates = VK_FALSE;

		ErrorUtils::VkAssert(vkCreateSampler(m_rootDevice->GetVkDevice(), &info, nullptr, &m_sampler), "VSampler");
	}

	VSampler::~VSampler()
	{
		Destroy();
	}

	void VSampler::Destroy()
	{
		if (m_sampler != VK_NULL_HANDLE)
		{
			vkDestroySampler(m_rootDevice->GetVkDevice(), m_sampler, nullptr);
			m_sampler = VK_NULL_HANDLE;
		}
	}
}
