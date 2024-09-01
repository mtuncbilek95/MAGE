#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Texture/TextureFormat.h"

namespace MAGE
{
	namespace GALUtils
	{
		static u32 GetTextureFormatSize(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::TF_Unknown:				return 0;
			case TextureFormat::TF_RGBA32_Float:		return 16;
			case TextureFormat::TF_RGBA32_UInt:			return 16;
			case TextureFormat::TF_RGBA32_Int:			return 16;

			case TextureFormat::TF_RGB32_Float:			return 12;
			case TextureFormat::TF_RGB32_UInt:			return 12;
			case TextureFormat::TF_RGB32_Int:			return 12;

			case TextureFormat::TF_RGBA16_Float:		return 8;
			case TextureFormat::TF_RGBA16_UNorm:		return 8;
			case TextureFormat::TF_RGBA16_UInt:			return 8;
			case TextureFormat::TF_RGBA16_Norm:			return 8;
			case TextureFormat::TF_RGBA16_Int:			return 8;

			case TextureFormat::TF_RG32_Float:			return 8;
			case TextureFormat::TF_RG32_UInt:			return 8;
			case TextureFormat::TF_RG32_Int:			return 8;

			case TextureFormat::TF_RGBA8_UNorm:			return 4;
			case TextureFormat::TF_RGBA8_UInt:			return 4;
			case TextureFormat::TF_RGBA8_Norm:			return 4;
			case TextureFormat::TF_RGBA8_Int:			return 4;

			case TextureFormat::TF_RG16_Float:			return 4;
			case TextureFormat::TF_RG16_UNorm:			return 4;
			case TextureFormat::TF_RG16_UInt:			return 4;
			case TextureFormat::TF_RG16_Norm:			return 4;
			case TextureFormat::TF_RG16_Int:			return 4;

			case TextureFormat::TF_D32_Float:			return 4;
			case TextureFormat::TF_R32_Float:			return 4;
			case TextureFormat::TF_R32_UInt:			return 4;
			case TextureFormat::TF_R32_Int:				return 4;

			case TextureFormat::TF_D24_UNorm_S8_UInt:	return 4;
			case TextureFormat::TF_D32_Float_S8_UInt:	return 5;

			case TextureFormat::TF_RG8_UNorm:			return 2;
			case TextureFormat::TF_RG8_UInt:			return 2;
			case TextureFormat::TF_RG8_Norm:			return 2;
			case TextureFormat::TF_RG8_Int:				return 2;

			case TextureFormat::TF_R16_Float:			return 2;
			case TextureFormat::TF_D16_UNorm:			return 2;
			case TextureFormat::TF_R16_UNorm:			return 2;
			case TextureFormat::TF_R16_UInt:			return 2;
			case TextureFormat::TF_R16_Norm:			return 2;
			case TextureFormat::TF_R16_Int:				return 2;

			case TextureFormat::TF_R8_UNorm:			return 1;
			case TextureFormat::TF_R8_UInt:				return 1;
			case TextureFormat::TF_R8_Norm:				return 1;
			case TextureFormat::TF_R8_Int:				return 1;

			default:									return 0;
			}
		}
	}
}
