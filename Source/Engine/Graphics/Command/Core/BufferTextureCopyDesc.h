#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	/**
	 * @struct BufferTextureCopyDesc
	 * @brief Mostly used for copying between host texture data and device buffers
	 */
	struct BufferTextureCopyDesc
	{
		u64 BufferOffsetInBytes;
		Math::Vec3i TextureOffset;
		Math::Vec3u TextureSize;
		u8 TargetArrayIndex;
		u8 TargetMipIndex;
	};
}