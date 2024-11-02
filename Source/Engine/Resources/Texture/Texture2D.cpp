#include "Texture2D.h"

namespace MAGE
{
	Texture2D::Texture2D() : Texture()
	{
	}

	Texture2D::~Texture2D()
	{
	}

	void Texture2D::CreateImage(const String& imagePath)
	{
		Texture::CreateImage(imagePath);

		ImageProps imageProp = {};
		imageProp.imageSize = { m_props.imageSize.x, m_props.imageSize.y, 1 };
		imageProp.mipLevels = 1;
		imageProp.arrayLayers = 1;
		imageProp.imageType = VK_IMAGE_TYPE_2D;
		imageProp.format = m_props.channelCount == 3 ? VK_FORMAT_R8G8B8_UNORM : VK_FORMAT_R8G8B8A8_UNORM;
		imageProp.imageUsage = VK_IMAGE_USAGE_SAMPLED_BIT;

		m_image = MakeOwned<VImage>(imageProp, m_context.GetDevice());
	}

	void Texture2D::CreateView()
	{
		ImageViewProps viewProp = {};
		viewProp.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewProp.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
		viewProp.baseMipLevel = 0;
		viewProp.baseArrayLayer = 0;

		m_view = m_image->CreateView(viewProp);
	}
}
