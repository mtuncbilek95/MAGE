#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Texture/TextureLayout.h"
#include "Engine/Graphics/Command/Core/ResolveModeFlags.h"
#include "Engine/Graphics/Command/Core/AttachmentLoadOperation.h"
#include "Engine/Graphics/Command/Core/AttachmentStoreOperation.h"

namespace MAGE
{
	class ENGINE_API TextureView;

	/**
	 * @struct DynamicPassAttachmentDesc
	 * @brief Describes a dynamic pass attachment
	 */
	struct DynamicPassAttachmentDesc
	{
		TextureView* ImageBuffer;
		TextureLayout ImageLayout;
		ResolveModeFlags ResolveMode;
		TextureView* ResolveBuffer;
		TextureLayout ResolveLayout;
		AttachmentLoadOperation LoadOperation;
		AttachmentStoreOperation StoreOperation;
		Math::Vec4f ClearColor;
		Math::Vec2f ClearDepthStencil;
	};
}
