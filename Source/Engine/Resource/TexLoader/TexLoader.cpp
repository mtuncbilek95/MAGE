#include "TexLoader.h"

#include "Engine/Platform/PlatformFile.h"

#include <stb_image.h>

namespace MAGE
{
	OwnedBuffer Importer::LoadTexture(const string& path)
	{
		if (!PlatformFile::Exists(path))
		{
			spdlog::error("File not found at : {}", path);
			return OwnedBuffer();
		}

		i32 width, height, channels;
		stbi_uc* image = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		if (!image)
		{
			spdlog::error("Failed to load image at : {}", path);
			return OwnedBuffer();
		}

		OwnedBuffer data = OwnedBuffer(image, width * height * channels);

		stbi_image_free(image);

		return std::move(data);
	}
}
