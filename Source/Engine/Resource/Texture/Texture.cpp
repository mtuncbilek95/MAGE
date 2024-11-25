#include "Texture.h"

#include "Engine/Platform/PlatformFile.h"
#include "Engine/RenderContext/RenderContext.h"

namespace MAGE
{
	Texture::Texture(const TextureProps& texDesc, const ResourceProps& resDesc) : Resource(resDesc), m_texProps(texDesc)
	{
	}

	void Texture::GenerateTexture()
	{
		BufferProps stageProp =
		{
			.sizeInBytes = static_cast<usize>(m_texProps.texSize.x * m_texProps.texSize.y * m_texProps.texSize.z * m_texProps.channelCount),
			.usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		};
		m_stage = MakeOwned<VBuffer>(stageProp, Context::GetMainDevice());
		m_stage->BindMemory(Context::GetMainAllocator()->GetAvailableMemory(AllocProps(m_stage->GetRequestedSize(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)));

		//ResourceHandler::ReadResourceFile(m_resProps.relativePath, m_resProps.data);
		m_stage->Update(m_resProps.data, 0);
	}

	bool Texture::Load()
	{
		return false;
	}

	bool Texture::Unload()
	{
		return false;
	}

	void Texture::Serialize(const path& absPath)
	{
		Resource::Serialize(absPath);


	}

	void Texture::Deserialize(const path& relPath)
	{
		Resource::Deserialize(relPath);
	}

	void Texture::Destroy()
	{
		if (m_stage)
			m_stage.reset();
		if (m_image)
			m_image.reset();
		if (m_view)
			m_view.reset();
	}
}
