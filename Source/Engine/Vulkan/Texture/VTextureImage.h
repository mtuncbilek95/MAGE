#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Texture/TextureImage.h"

#include <vulkan/vulkan.h>

namespace MAGE
{
	class VDevice;
	class VTextureImage : public TextureImage
	{
	public:
		VTextureImage(const TextureImageDesc& desc, VDevice* pDevice);
		VTextureImage(const TextureImageDesc& desc, VkImage texture, VDevice* pDevice);
		~VTextureImage() override;

		VkImage GetVkTexture() const { return mTexture; }
		VkDevice GetVkDevice() const { return mDevice; }

		u64 GetVkOffset() const { return mOffset; }
		u64 GetVkAlignedOffset() const { return mAlignedOffset; }

		void NullifyTexture() { mTexture = VK_NULL_HANDLE; }

	private:
		VkImage mTexture;
		VkDevice mDevice;

		u64 mOffset;
		u64 mAlignedOffset;
	};
}
