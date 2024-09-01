#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class TextureViewType : u8
	{
		TVT_Undefined,
		TVT_Texture1D,
		TVT_Texture2D,
		TVT_Texture3D,
		TVT_TextureCube,
		TVT_Texture1DArray,
		TVT_Texture2DArray,
		TVT_TextureCubeArray
	};
}