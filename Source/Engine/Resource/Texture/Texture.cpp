#include "Texture.h"

#include "Engine/Platform/PlatformFile.h"
#include "Engine/RenderContext/RenderContext.h"

namespace MAGE
{
	Texture::Texture(const json& desc) : Resource(desc)
	{
	}

	Texture::~Texture() = default;

	void Texture::GenerateBuffer()
	{
		/*BufferProps stageProp =
		{
			.sizeInBytes = static_cast<usize>(m_texProps.texSize.x * m_texProps.texSize.y * m_texProps.texSize.z * m_texProps.channelCount),
			.usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		};
		m_stage = MakeOwned<VBuffer>(stageProp, Context::GetMainDevice());
		m_stage->BindMemory(Context::GetMainAllocator()->GetAvailableMemory(AllocProps(m_stage->GetRequestedSize(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)));*/
		//m_stage->Update(m_resProps.data, 0);
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
			m_stage.reset();
		if (m_image)
			m_image.reset();
		if (m_view)
			m_view.reset();
	}
}
