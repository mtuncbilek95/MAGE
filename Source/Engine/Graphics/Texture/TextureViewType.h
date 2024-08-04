#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	enum class TextureViewType : u8
	{
		Undefined,
		Texture1D,
		Texture2D,
		Texture3D,
		TextureCube,
		Texture1DArray,
		Texture2DArray,
		TextureCubeArray
	};
}