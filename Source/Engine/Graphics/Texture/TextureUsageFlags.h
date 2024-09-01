#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class TextureUsageFlags : u16
	{
		TUF_None = 0,
		TUF_SampledImage = 1 << 0,
		TUF_StorageImage = 1 << 1,
		TUF_ColorAttachment = 1 << 2,
		TUF_DepthStencilAttachment = 1 << 3,
		TUF_TransientAttachment = 1 << 4,
		TUF_InputAttachment = 1 << 5,
		TUF_TransferSource = 1 << 6,
		TUF_TransferDestination = 1 << 7
	};

	GENERATE_ENUM_FLAG(TextureUsageFlags, u16);
}
