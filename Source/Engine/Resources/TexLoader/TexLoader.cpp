#include "TexLoader.h"

#include "Engine/IO/PlatformFile.h"

#include <stb_image.h>

namespace MAGE
{
	TexData Importer::LoadTexture(const String& path)
	{
		if (!PlatformFile::Exists(path))
		{
			spdlog::error("File not found at : {}", path.CharString());
			return TexData();
		}

		TexData data;

		i32 width, height, channels;
		stbi_uc* image = stbi_load(path.CharString(), &width, &height, &channels, STBI_rgb_alpha);

		if (!image)
		{
			spdlog::error("Failed to load image at : {}", path.CharString());
			return TexData();
		}

		data.imageSize = { static_cast<u32>(width), static_cast<u32>(height) };
		data.channelCount = channels;
		data.data = OwnedBuffer(image, width * height * channels);

		stbi_image_free(image);

		return data;
	}
}
