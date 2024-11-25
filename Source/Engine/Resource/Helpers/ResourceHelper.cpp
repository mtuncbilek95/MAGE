#include "ResourceHelper.h"

#include "Engine/Configuration/ConfigCreator.h"
#include "Engine/Platform/PlatformFile.h"

#include <stb_image.h>

namespace MAGE
{

	const map<string, ResourceType> resourceMap =
	{
		{".png", ResourceType::Texture},
		{".tga", ResourceType::Texture},
		{".dds", ResourceType::Texture},

		{".gltf", ResourceType::Mesh},
		{".fbx", ResourceType::Mesh},

		{".vert", ResourceType::Shader},
		{".frag", ResourceType::Shader},
		{".comp", ResourceType::Shader},
		{".geom", ResourceType::Shader},
		{".tesc", ResourceType::Shader},
		{".tese", ResourceType::Shader},
		{".rgen", ResourceType::Shader},
		{".rint", ResourceType::Shader},
		{".rahit", ResourceType::Shader},
		{".rchit", ResourceType::Shader},
		{".rmiss", ResourceType::Shader},
		{".rcall", ResourceType::Shader},
		{".task", ResourceType::Shader},
		{".mesh", ResourceType::Shader},

		{".ttf", ResourceType::Font},
		{".otf", ResourceType::Font},

		{".wav", ResourceType::Audio},
		{".flac", ResourceType::Audio},

		{".cpp", ResourceType::Script},
		{".h", ResourceType::Script},
	};

	static ResourceType GetResourceType(const path& ext)
	{
		auto it = resourceMap.find(ext.extension().string());
		if (it != resourceMap.end())
			return it->second;
			 
		spdlog::critical("Unknown resource type: {}", ext.extension().string());
		return ResourceType::Unknown;
	}

	void ResourceHandler::ReadFileFromDisk(const string& aPath, const string& relExtPath, ResourceProps& props)
	{
		path newPath = aPath;
		props.resourceName = newPath.stem().string();
		props.relativePath = (Config::GetAppConfig().projectPath / relExtPath).string();
		props.guid = Guid::GenerateID();
		props.lastModified = DayTime::GetCurrentTime();
		props.type = GetResourceType(newPath);
		props.sizeInBytes = std::filesystem::file_size(newPath);

		switch (props.type)
		{
			case ResourceType::Texture:
				// Read with framework
				// Write as binary
				break;
			case ResourceType::Mesh:
				// Read with framework
				// Write as binary
				break;
			case ResourceType::Shader:
				// Read as string
				// write as binary
				break;
			case ResourceType::Script:
				// Copy
				break;
			case ResourceType::Font:
				// Read with framework
				// Write as binary
				break;
			case ResourceType::Audio:
				// Read with framework
				// Write as binary
				break;
			default:
				spdlog::critical("You fucked up!");
				break;
		}
	}

	void ResourceHandler::ReadResourceFile(const string& relPath, OwnedBuffer& buffer)
	{
		path newPath = Config::GetAppConfig().projectPath;
		newPath /= relPath;

		if (!PlatformFile::Read(newPath.string(), buffer))
			spdlog::error("Failed to load from {}", relPath);
	}

	void ResourceHandler::ReadTextureDisk(const string& fPath, Imagery& buffer)
	{
		// TODO: Need better reader.
		int width, height, channels;
		stbi_uc* data = stbi_load(fPath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		buffer.channelCount = channels;
		buffer.imageSize = { static_cast<u32>(width), static_cast<u32>(height) };
		buffer.buffer = OwnedBuffer(data, static_cast<u64>(width * height * 8 * channels));
	}

	void ResourceHandler::ReadGeometryDisk(const string& fPath, Geometry& buffer)
	{
	}
}
