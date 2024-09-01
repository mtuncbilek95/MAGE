#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Texture/TextureAspectFlags.h"
#include "Engine/Graphics/Texture/TextureViewType.h"

namespace MAGE
{
	class TextureImage;
	struct TextureViewDesc
	{
		u8 MipLevel;
		u8 MipCount;
		u8 ArrayLayer;
		u8 ArrayCount;
		TextureAspectFlags AspectFlags;
		TextureViewType ViewType;
		TextureImage* pTexture;
	};
}
