#include "VDescSet.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Device/VDevice.h"
#include "../Descriptor/VDescLayout.h"
#include "../Descriptor/VDescPool.h"
#include "../Image/VImageView.h"
#include "../Buffer/VDstBuffer.h"
#include "../Sampler/VSampler.h"

namespace MAGE
{
	VDescSet::VDescSet(const DescSetProps& desc, VDevice* device) : VObject(device), m_props(desc)
	{
		VkDescriptorSetLayout dLayout = desc.layout->GetLayout();

		VkDescriptorSetAllocateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		info.descriptorSetCount = 1;
		info.descriptorPool = desc.pool->GetPool();
		info.pSetLayouts = &dLayout;

		ErrorUtils::VkAssert(vkAllocateDescriptorSets(m_rootDevice->GetDevice(), &info, &m_set), "VDescSet");
	}

	VDescSet::~VDescSet()
	{
		Destroy();
	}

	void VDescSet::UpdateSet(const UpdateProps& desc) const
	{
		VkWriteDescriptorSet writeInformations[32];
		VkDescriptorBufferInfo writeBufferInformations[32];
		VkDescriptorImageInfo writeImageInformations[32];
		u32 bufferIndex = 0;
		u32 imageIndex = 0;

		for (u8 i = 0; i < desc.entries.size(); i++)
		{
			VkWriteDescriptorSet writeInfo = {};
			writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeInfo.dstSet = m_set;
			writeInfo.dstBinding = desc.entries[i].binding;
			writeInfo.dstArrayElement = desc.entries[i].arrayElement;
			writeInfo.descriptorCount = desc.entries[i].count;
			writeInfo.descriptorType = desc.entries[i].type;
			writeInfo.pNext = nullptr;

			switch (desc.entries[i].type)
			{
			case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
			{
				VkDescriptorImageInfo imageInfo = {};
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo.imageView = desc.entries[i].entry.image->GetView();
				imageInfo.sampler = desc.entries[i].sampler->GetSampler();
				writeImageInformations[imageIndex] = imageInfo;
				writeInfo.pImageInfo = &writeImageInformations[imageIndex];
				writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				imageIndex++;
				break;
			}
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
			{
				VkDescriptorBufferInfo bufferInfo = {};
				bufferInfo.buffer = desc.entries[i].entry.uniform->GetBuffer();
				bufferInfo.offset = desc.entries[i].bufferOffset;
				bufferInfo.range = desc.entries[i].entry.uniform->GetTotalSize();
				writeBufferInformations[bufferIndex] = bufferInfo;
				writeInfo.pBufferInfo = &writeBufferInformations[bufferIndex];
				writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				bufferIndex++;
				break;
			}
			default:
				spdlog::warn("{} is not a valid descriptor type.", magic_enum::enum_name<VkDescriptorType>(desc.entries[i].type));
			}

			writeInformations[i] = writeInfo;
		}

		vkUpdateDescriptorSets(m_rootDevice->GetDevice(), desc.entries.size(), writeInformations, 0, nullptr);
	}

	void VDescSet::Destroy()
	{
		if (m_set != VK_NULL_HANDLE)
		{
			vkFreeDescriptorSets(m_rootDevice->GetDevice(), m_props.pool->GetPool(), 1, &m_set);
			m_set = VK_NULL_HANDLE;
		}
	}
}
