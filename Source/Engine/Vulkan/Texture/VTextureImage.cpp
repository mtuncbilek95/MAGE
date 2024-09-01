#include "VTextureImage.h"
#include "VTextureUtils.h"

#include "Engine/Vulkan/Device/VDevice.h"
#include "Engine/Vulkan/Memory/VMemory.h"

#include "Engine/Platform/PlatformErrorMessage.h"

namespace MAGE
{
	VTextureImage::VTextureImage(const TextureImageDesc& desc, VDevice* pDevice) : TextureImage(desc, pDevice),
		mTexture(VK_NULL_HANDLE), mDevice(pDevice->GetVkDevice()), mOffset(0), mAlignedOffset(0)
	{
		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VkUtils::GetVkImageType(desc.ImageType);
		imageInfo.extent.width = desc.ImageSize.x;
		imageInfo.extent.height = desc.ImageSize.y;
		imageInfo.extent.depth = desc.ImageSize.z;
		imageInfo.mipLevels = desc.MipLevels;
		imageInfo.arrayLayers = desc.ArrayLayers;
		
		auto format = VkUtils::VkFormatMap.find(desc.ImageFormat);
		MAGE_ASSERT(format != VkUtils::VkFormatMap.end(), "VTextureImage", "Failed to find format!");
		imageInfo.format = format->second;

		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VkUtils::GetVkImageUsageFlags(desc.UsageFlags);
		imageInfo.samples = VkUtils::GetVkSampleCountFlags(desc.Samples);
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.flags = VkUtils::GetVkImageCreateFlags(desc.CreateFlags);

		MAGE_ASSERT(vkCreateImage(mDevice, &imageInfo, nullptr, &mTexture) == VK_SUCCESS, "VTextureImage", "Failed to create image!");

		VkMemoryRequirements info = {};
		vkGetImageMemoryRequirements(mDevice, mTexture, &info);

		u64 memoryOffset = desc.pRequestMemory->Allocate(info.size + info.alignment);
		u64 alignedOffset = memoryOffset + (memoryOffset % info.alignment == 0 ? 0 : (info.alignment - (memoryOffset % info.alignment)));

		VMemory* memPtr = desc.pRequestMemory->GetAs<VMemory>();
		MAGE_ASSERT(vkBindImageMemory(mDevice, mTexture, memPtr->GetVkDeviceMemory(), alignedOffset) == VK_SUCCESS, "VTextureImage", "Failed to bind image memory!");

		mOffset = memoryOffset;
		mAlignedOffset = alignedOffset;
	}

	VTextureImage::VTextureImage(const TextureImageDesc& desc, VkImage texture, VDevice* pDevice) : TextureImage(desc, pDevice),
		mTexture(texture), mDevice(pDevice->GetVkDevice()), mOffset(0), mAlignedOffset(0)
	{
	}

	VTextureImage::~VTextureImage()
	{
		if(mTexture != VK_NULL_HANDLE)
			vkDestroyImage(mDevice, mTexture, nullptr);
	}
}
