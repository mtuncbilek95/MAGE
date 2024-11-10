#include "VDescSet.h"

#include "../Device/VDevice.h"
#include "../Descriptor/VDescLayout.h"
#include "../Descriptor/VDescPool.h"
#include "../Image/VImageView.h"
#include "../Buffer/VBuffer.h"
#include "../Sampler/VSampler.h"

#include "Engine/Vulkan/Core/VkAssert.h"

#include <magic_enum.hpp>

namespace MAGE
{
	VDescSet::VDescSet(const DescSetProps& desc, VDevice* device) : VkObject(device), m_props(desc)
	{
		vk::DescriptorSetLayout dLayout = desc.layout->GetVkLayout();

		vk::DescriptorSetAllocateInfo info = {};
		info.descriptorSetCount = 1;
		info.descriptorPool = desc.pool->GetVkPool();
		info.pSetLayouts = &dLayout;

		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().allocateDescriptorSets(&info, &m_set), "VDescSet");
	}

	VDescSet::~VDescSet()
	{
		Destroy();
	}

	void VDescSet::UpdateData(const SetUpdateProps& desc)
	{
		vk::WriteDescriptorSet writeInformations[32];
		vk::DescriptorBufferInfo writeBufferInformations[32];
		vk::DescriptorImageInfo writeImageInformations[32];
		u32 bufferIndex = 0;
		u32 imageIndex = 0;

		for (u8 i = 0; i < desc.entries.size(); i++)
		{
			vk::WriteDescriptorSet writeInfo = {};
			writeInfo.dstSet = m_set;
			writeInfo.dstBinding = desc.entries[i].binding;
			writeInfo.dstArrayElement = desc.entries[i].arrayElement;
			writeInfo.descriptorCount = desc.entries[i].count;
			writeInfo.descriptorType = desc.entries[i].type;
			writeInfo.pNext = nullptr;

			switch (desc.entries[i].type)
			{
			case vk::DescriptorType::eCombinedImageSampler:
			{
				vk::DescriptorImageInfo imageInfo = {};
				imageInfo.imageView = desc.entries[i].entry.image.image->GetVkView();
				imageInfo.sampler = desc.entries[i].entry.image.sampler->GetVkSampler();
				writeImageInformations[imageIndex] = imageInfo;
				writeInfo.pImageInfo = &writeImageInformations[imageIndex];
				writeInfo.descriptorType = vk::DescriptorType::eCombinedImageSampler;
				imageIndex++;
				break;
			}
			case vk::DescriptorType::eUniformBuffer:
			{
				vk::DescriptorBufferInfo bufferInfo = {};
				bufferInfo.buffer = desc.entries[i].entry.uniform->GetVkBuffer();
				bufferInfo.offset = desc.entries[i].bufferOffset;
				bufferInfo.range = desc.entries[i].entry.uniform->GetTotalSize();
				writeBufferInformations[bufferIndex] = bufferInfo;
				writeInfo.pBufferInfo = &writeBufferInformations[bufferIndex];
				writeInfo.descriptorType = vk::DescriptorType::eUniformBuffer;
				bufferIndex++;
				break;
			}
			default:
				spdlog::warn("{} is not a valid descriptor type.", magic_enum::enum_name<vk::DescriptorType>(desc.entries[i].type));
			}

			writeInformations[i] = writeInfo;
		}

		m_rootDevice->GetVkDevice().updateDescriptorSets(desc.entries.size(), writeInformations, 0, nullptr);
	}

	void VDescSet::Destroy()
	{
		if (m_set != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().freeDescriptorSets(m_props.pool->GetVkPool(), 1, &m_set);
			m_set = VK_NULL_HANDLE;
		}
	}
}
