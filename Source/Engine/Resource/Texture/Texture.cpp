#include "Texture.h"

#include "Engine/Platform/PlatformFile.h"
#include "Engine/Resource/TexLoader/TexLoader.h"
#include "Engine/RenderContext/RenderContext.h"

namespace MAGE
{
	Texture::Texture()
	{
	}

	Texture::~Texture()
	{
		Destroy();
	}

	void Texture::LoadTexture()
	{
		if (!PlatformFile::Exists(m_resProps.absolutePath))
		{
			spdlog::error("\"{}\" is not a valid path.", m_resProps.absolutePath);
			return;
		}

		BufferProps stageProp =
		{
			.sizeInBytes = static_cast<usize>(m_texProps.texSize.x * m_texProps.texSize.y * m_texProps.texSize.z * m_texProps.channelCount),
			.usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		};
		m_stage = MakeOwned<VBuffer>(stageProp, Context::GetMainDevice());
		m_stage->BindMemory(Context::GetMainAllocator()->GetAvailableMemory(AllocProps(m_stage->GetRequestedSize(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)));

		OwnedBuffer buffer;
		PlatformFile::Read(m_resProps.absolutePath, buffer);
		m_stage->Update(buffer, 0);
	}

	void Texture::UnloadTexture()
	{
	}

	void Texture::Serialize()
	{
	}

	void Texture::Deserialize()
	{
	}

	void Texture::Destroy()
	{
		if (m_stage)
			m_stage->Destroy();

		if (m_image)
			m_image->Destroy();

		if (m_view)
			m_view->Destroy();
	}
}
