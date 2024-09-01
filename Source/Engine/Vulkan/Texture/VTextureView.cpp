#include "VTextureView.h"
#include "VTextureUtils.h"

#include "Engine/Vulkan/Device/VDevice.h"
#include "Engine/Vulkan/Texture/VTextureImage.h"
#include "Engine/Platform/PlatformErrorMessage.h"

namespace MAGE
{
	VTextureView::VTextureView(const TextureViewDesc& desc, VDevice* pDevice) : TextureView(desc, pDevice), mDevice(pDevice->GetVkDevice())
	{
		VkImageViewCreateInfo imageViewInfo = {};
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.image = desc.pTexture->GetAs<VTextureImage>()->GetVkTexture();
		imageViewInfo.viewType = VkUtils::GetVkTextureViewType(desc.ViewType);

		auto format = VkUtils::VkFormatMap.find(desc.pTexture->GetImageFormat());
		Helpers::MageAssert(format != VkUtils::VkFormatMap.end(), "VTextureImage", "Failed to find format!");
		imageViewInfo.format = format->second;

		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.subresourceRange.aspectMask = VkUtils::GetVkTextureAspectFlags(desc.AspectFlags);
		imageViewInfo.subresourceRange.baseMipLevel = desc.MipLevel;
		imageViewInfo.subresourceRange.levelCount = desc.MipCount;
		imageViewInfo.subresourceRange.baseArrayLayer = desc.ArrayLayer;
		imageViewInfo.subresourceRange.layerCount = desc.ArrayCount;

		Helpers::MageAssert(vkCreateImageView(mDevice, &imageViewInfo, nullptr, &mImageView) == VK_SUCCESS, "VTextureView", "Failed to create image view!");
	}

	VTextureView::~VTextureView()
	{
		vkDestroyImageView(mDevice, mImageView, nullptr);
	}
}
