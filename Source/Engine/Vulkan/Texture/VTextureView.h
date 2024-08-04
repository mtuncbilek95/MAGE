#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Texture/TextureView.h>

#include <vulkan/vulkan.h>

namespace MAGE
{
	class VDevice;
	class VTextureView : public TextureView
	{
	public:
		VTextureView(const TextureViewDesc& desc, VDevice* pDevice);
		~VTextureView() override;

		VkImageView GetVkTextureView() const { return mImageView; }
		VkDevice GetVkDevice() const { return mDevice; }

	private:
		VkImageView mImageView;
		VkDevice mDevice;
	};
}
