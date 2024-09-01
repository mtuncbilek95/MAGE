#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Command/Core/DynamicPassAttachmentDesc.h"

namespace MAGE
{
	struct DynamicPassDesc
	{
		Vec2u Extent;
		Vec2i Offset;
		u32 layerCount;
		u32 viewMask;
		Vector<DynamicPassAttachmentDesc> ColorAttachments;
		DynamicPassAttachmentDesc DepthAttachment;
		DynamicPassAttachmentDesc StencilAttachment;
	};
}
