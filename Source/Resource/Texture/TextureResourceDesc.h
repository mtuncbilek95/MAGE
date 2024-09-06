#pragma once

#include "Engine/Graphics/Texture/TextureFormat.h"
#include "Engine/Graphics/Texture/TextureUsageFlags.h"
#include "Engine/Graphics/Texture/TextureType.h"
#include "Engine/Graphics/Texture/SampleCount.h"
#include "Engine/Graphics/Texture/TextureCreateFlags.h"

namespace MAGE
{
	struct TextureResourceDesc
	{
		Math::Vec3u ImageSize;
		TextureFormat ImageFormat;
		TextureType ImageType;
		SampleCount Samples;
		TextureCreateFlags CreateFlags;
	};
}
