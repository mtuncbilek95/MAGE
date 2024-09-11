#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class ENGINE_API TextureLayout : u16
	{
		TL_Undefined,
		TL_General,
		TL_ColorAttachment,
		TL_DepthStencilAttachment,
		TL_ShaderReadOnly,
		TL_TransferSource,
		TL_TransferDestination,
		TL_DepthAttachment,
		TL_Present
	};
}
