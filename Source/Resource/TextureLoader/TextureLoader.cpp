#include "TextureLoader.h"

#include <Engine/Platform/PlatformFile.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace MAGE
{
	namespace Importer
	{
		TextureMemory LoadTexture(const String& path)
		{
			int width, height, numChannels;
			u8* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);

			if(!data)
			{
				CORE_LOG(M_ERROR, "Failed to load image: %s", path.c_str());
				return TextureMemory();
			}

			TextureMemory textureMemory;
			textureMemory.Data = MemoryOwnedBuffer(data, width * height * numChannels);
			textureMemory.ImageRes = Vec2u(width, height);
			textureMemory.NumChannel = numChannels;

			return textureMemory;
		}
	}
}