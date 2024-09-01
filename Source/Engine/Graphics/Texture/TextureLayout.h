#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class TextureLayout : u16
	{
		Undefined,
		General,
		ColorAttachment,
		DepthStencilAttachment,
		ShaderReadOnly,
		TransferSource,
		TransferDestination,
		DepthAttachment,
		Present
	};
}
