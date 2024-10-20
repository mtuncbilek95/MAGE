#include "VAssert.h"

namespace MAGE
{
	HashMap<VkFormat, u32> MakeFormatSizeMap()
	{
		HashMap<VkFormat, u32> map;
		for (auto format : magic_enum::enum_values<VkFormat>())
		{
			u32 size = 0;
			if (format == VK_FORMAT_R4G4_UNORM_PACK8 || format == VK_FORMAT_S8_UINT) size = 1;
			else if (format >= VK_FORMAT_R4G4B4A4_UNORM_PACK16 && format <= VK_FORMAT_A1R5G5B5_UNORM_PACK16)
				size = 2;
			else if (format >= VK_FORMAT_R8_UNORM && format <= VK_FORMAT_R8_SRGB) size = 1;
			else if (format >= VK_FORMAT_R8G8_UNORM && format <= VK_FORMAT_R8G8_SRGB) size = 2;
			else if (format >= VK_FORMAT_R8G8B8_UNORM && format <= VK_FORMAT_B8G8R8_SRGB) size = 3;
			else if (format >= VK_FORMAT_R8G8B8A8_UNORM && format <= VK_FORMAT_A2B10G10R10_SINT_PACK32)
				size = 4;
			else if (format >= VK_FORMAT_R16_UNORM && format <= VK_FORMAT_R16_SFLOAT) size = 2;
			else if (format >= VK_FORMAT_R16G16_UNORM && format <= VK_FORMAT_R16G16_SFLOAT) size = 4;
			else if (format >= VK_FORMAT_R16G16B16_UNORM && format <= VK_FORMAT_R16G16B16_SFLOAT) size = 6;
			else if (format >= VK_FORMAT_R16G16B16A16_UNORM && format <= VK_FORMAT_R16G16B16A16_SFLOAT)
				size = 8;
			else if (format >= VK_FORMAT_R32_UINT && format <= VK_FORMAT_R32_SFLOAT) size = 4;
			else if (format >= VK_FORMAT_R32G32_UINT && format <= VK_FORMAT_R32G32_SFLOAT) size = 8;
			else if (format >= VK_FORMAT_R32G32B32_UINT && format <= VK_FORMAT_R32G32B32_SFLOAT) size = 12;
			else if (format >= VK_FORMAT_R32G32B32A32_UINT && format <= VK_FORMAT_R32G32B32A32_SFLOAT)
				size = 16;
			else if (format >= VK_FORMAT_R64_UINT && format <= VK_FORMAT_R64_SFLOAT) size = 8;
			else if (format >= VK_FORMAT_R64G64_UINT && format <= VK_FORMAT_R64G64_SFLOAT) size = 16;
			else if (format >= VK_FORMAT_R64G64B64_UINT && format <= VK_FORMAT_R64G64B64_SFLOAT) size = 24;
			else if (format >= VK_FORMAT_R64G64B64A64_UINT && format <= VK_FORMAT_R64G64B64A64_SFLOAT)
				size = 32;
			else if (format == VK_FORMAT_B10G11R11_UFLOAT_PACK32 || format == VK_FORMAT_E5B9G9R9_UFLOAT_PACK32
				|| format == VK_FORMAT_X8_D24_UNORM_PACK32 || format == VK_FORMAT_D32_SFLOAT
				|| format == VK_FORMAT_D24_UNORM_S8_UINT)
				size = 4;
			else if (format == VK_FORMAT_D16_UNORM) size = 2;
			else if (format == VK_FORMAT_D16_UNORM_S8_UINT) size = 3;
			else if (format == VK_FORMAT_D32_SFLOAT_S8_UINT) size = 5;
			else if (format == VK_FORMAT_BC1_RGB_UNORM_BLOCK || format == VK_FORMAT_BC1_RGB_SRGB_BLOCK
				|| format == VK_FORMAT_BC1_RGBA_UNORM_BLOCK || format == VK_FORMAT_BC1_RGBA_SRGB_BLOCK
				|| format == VK_FORMAT_BC4_SNORM_BLOCK || format == VK_FORMAT_BC4_UNORM_BLOCK)
				size = 8;
			else if (format == VK_FORMAT_BC2_SRGB_BLOCK || format == VK_FORMAT_BC2_UNORM_BLOCK
				|| format == VK_FORMAT_BC3_SRGB_BLOCK || format == VK_FORMAT_BC3_UNORM_BLOCK
				|| format == VK_FORMAT_BC5_SNORM_BLOCK || format == VK_FORMAT_BC5_UNORM_BLOCK
				|| format == VK_FORMAT_BC6H_SFLOAT_BLOCK || format == VK_FORMAT_BC6H_UFLOAT_BLOCK
				|| format == VK_FORMAT_BC7_SRGB_BLOCK || format == VK_FORMAT_BC7_UNORM_BLOCK)
				size = 16;
			else if (format == VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK || format == VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK
				|| format == VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK
				|| format == VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK
				|| format == VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK
				|| format == VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK || format == VK_FORMAT_EAC_R11_UNORM_BLOCK
				|| format == VK_FORMAT_EAC_R11_SNORM_BLOCK || format == VK_FORMAT_EAC_R11G11_UNORM_BLOCK
				|| format == VK_FORMAT_EAC_R11G11_SNORM_BLOCK)
				size = 16;
			else if (format == VK_FORMAT_ASTC_4x4_UNORM_BLOCK || format == VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK
				|| format == VK_FORMAT_ASTC_4x4_SRGB_BLOCK || format == VK_FORMAT_ASTC_5x4_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK || format == VK_FORMAT_ASTC_5x4_SRGB_BLOCK
				|| format == VK_FORMAT_ASTC_5x5_UNORM_BLOCK || format == VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK
				|| format == VK_FORMAT_ASTC_5x5_SRGB_BLOCK || format == VK_FORMAT_ASTC_6x5_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK || format == VK_FORMAT_ASTC_6x5_SRGB_BLOCK
				|| format == VK_FORMAT_ASTC_6x6_UNORM_BLOCK || format == VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK
				|| format == VK_FORMAT_ASTC_6x6_SRGB_BLOCK || format == VK_FORMAT_ASTC_8x5_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK || format == VK_FORMAT_ASTC_8x5_SRGB_BLOCK
				|| format == VK_FORMAT_ASTC_8x6_UNORM_BLOCK || format == VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK
				|| format == VK_FORMAT_ASTC_8x6_SRGB_BLOCK || format == VK_FORMAT_ASTC_8x8_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK || format == VK_FORMAT_ASTC_8x8_SRGB_BLOCK
				|| format == VK_FORMAT_ASTC_10x5_UNORM_BLOCK || format == VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK
				|| format == VK_FORMAT_ASTC_10x5_SRGB_BLOCK || format == VK_FORMAT_ASTC_10x6_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK || format == VK_FORMAT_ASTC_10x6_SRGB_BLOCK
				|| format == VK_FORMAT_ASTC_10x8_UNORM_BLOCK || format == VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK
				|| format == VK_FORMAT_ASTC_10x8_SRGB_BLOCK || format == VK_FORMAT_ASTC_10x10_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK || format == VK_FORMAT_ASTC_10x10_SRGB_BLOCK
				|| format == VK_FORMAT_ASTC_12x10_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK || format == VK_FORMAT_ASTC_12x10_SRGB_BLOCK
				|| format == VK_FORMAT_ASTC_12x12_UNORM_BLOCK || format == VK_FORMAT_ASTC_12x12_SRGB_BLOCK
				|| format == VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK)
				size = 16;
			else if (format == VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG
				|| format == VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG
				|| format == VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG
				|| format == VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG
				|| format == VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG
				|| format == VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG
				|| format == VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG
				|| format == VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG)
				size = 8;
			else if (format == VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16
				|| format == VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16
				|| format == VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16
				|| format == VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16
				|| format == VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16
				|| format == VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16
				|| format == VK_FORMAT_G16B16G16R16_422_UNORM
				|| format == VK_FORMAT_B16G16R16G16_422_UNORM)
				size = 8;
			else if (format == VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16
				|| format == VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16
				|| format == VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16
				|| format == VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16
				|| format == VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16)
				size = 6;

			map[format] = size;
		}
		return map;
	}

	HashMap<VkFormat, Math::Vec2uc> MakeBlockSizeMap()
	{
		HashMap<VkFormat, Math::Vec2uc> map;

		for (auto format : magic_enum::enum_values<VkFormat>())
		{
			Math::Vec2u size = { 1, 1 };
			if (format == VK_FORMAT_BC1_RGB_UNORM_BLOCK || format == VK_FORMAT_BC1_RGB_SRGB_BLOCK
				|| format == VK_FORMAT_BC1_RGBA_UNORM_BLOCK || format == VK_FORMAT_BC1_RGBA_SRGB_BLOCK
				|| format == VK_FORMAT_BC2_UNORM_BLOCK || format == VK_FORMAT_BC2_SRGB_BLOCK
				|| format == VK_FORMAT_BC3_UNORM_BLOCK || format == VK_FORMAT_BC3_SRGB_BLOCK
				|| format == VK_FORMAT_BC4_UNORM_BLOCK || format == VK_FORMAT_BC4_SNORM_BLOCK
				|| format == VK_FORMAT_BC5_UNORM_BLOCK || format == VK_FORMAT_BC5_SNORM_BLOCK
				|| format == VK_FORMAT_BC6H_UFLOAT_BLOCK || format == VK_FORMAT_BC6H_SFLOAT_BLOCK
				|| format == VK_FORMAT_BC7_UNORM_BLOCK || format == VK_FORMAT_BC7_SRGB_BLOCK
				|| format == VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK || format == VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK
				|| format == VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK || format == VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK
				|| format == VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK || format == VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK
				|| format == VK_FORMAT_EAC_R11_UNORM_BLOCK || format == VK_FORMAT_EAC_R11G11_UNORM_BLOCK
				|| format == VK_FORMAT_EAC_R11G11_SNORM_BLOCK || format == VK_FORMAT_ASTC_4x4_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_4x4_SRGB_BLOCK || format == VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG
				|| format == VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG
				|| format == VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG || format == VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK)
				size = { 4, 4 };
			else if (format == VK_FORMAT_ASTC_5x4_UNORM_BLOCK || format == VK_FORMAT_ASTC_5x4_SRGB_BLOCK)
				size = { 5, 4 };
			else if (format == VK_FORMAT_ASTC_5x5_UNORM_BLOCK || format == VK_FORMAT_ASTC_5x5_SRGB_BLOCK)
				size = { 5, 5 };
			else if (format == VK_FORMAT_ASTC_6x5_UNORM_BLOCK || format == VK_FORMAT_ASTC_6x5_SRGB_BLOCK)
				size = { 6, 5 };
			else if (format == VK_FORMAT_ASTC_6x6_UNORM_BLOCK || format == VK_FORMAT_ASTC_6x6_SRGB_BLOCK)
				size = { 6, 6 };
			else if (format == VK_FORMAT_ASTC_8x5_UNORM_BLOCK || format == VK_FORMAT_ASTC_8x5_SRGB_BLOCK)
				size = { 8, 5 };
			else if (format == VK_FORMAT_ASTC_8x6_UNORM_BLOCK || format == VK_FORMAT_ASTC_8x6_SRGB_BLOCK)
				size = { 8, 6 };
			else if (format == VK_FORMAT_ASTC_8x8_UNORM_BLOCK || format == VK_FORMAT_ASTC_8x8_SRGB_BLOCK)
				size = { 8, 8 };
			else if (format == VK_FORMAT_ASTC_10x5_UNORM_BLOCK || format == VK_FORMAT_ASTC_10x5_SRGB_BLOCK)
				size = { 10, 5 };
			else if (format == VK_FORMAT_ASTC_10x6_UNORM_BLOCK || format == VK_FORMAT_ASTC_10x6_SRGB_BLOCK)
				size = { 10, 6 };
			else if (format == VK_FORMAT_ASTC_10x8_UNORM_BLOCK || format == VK_FORMAT_ASTC_10x8_SRGB_BLOCK)
				size = { 10, 8 };
			else if (format == VK_FORMAT_ASTC_10x10_UNORM_BLOCK || format == VK_FORMAT_ASTC_10x10_SRGB_BLOCK)
				size = { 10, 10 };
			else if (format == VK_FORMAT_ASTC_12x10_UNORM_BLOCK || format == VK_FORMAT_ASTC_12x10_SRGB_BLOCK)
				size = { 12, 10 };
			else if (format == VK_FORMAT_ASTC_12x12_UNORM_BLOCK || format == VK_FORMAT_ASTC_12x12_SRGB_BLOCK)
				size = { 12, 12 };
			else if (format == VK_FORMAT_G8B8G8R8_422_UNORM || format == VK_FORMAT_B8G8R8G8_422_UNORM
				|| format == VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM
				|| format == VK_FORMAT_G8_B8R8_2PLANE_422_UNORM
				|| format == VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16
				|| format == VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16
				|| format == VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16
				|| format == VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16
				|| format == VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16
				|| format == VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16
				|| format == VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16
				|| format == VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16
				|| format == VK_FORMAT_G16B16G16R16_422_UNORM || format == VK_FORMAT_B16G16R16G16_422_UNORM
				|| format == VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM
				|| format == VK_FORMAT_G16_B16R16_2PLANE_422_UNORM)
				size = { 2, 1 };
			else if (format == VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM
				|| format == VK_FORMAT_G8_B8R8_2PLANE_420_UNORM
				|| format == VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16
				|| format == VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16
				|| format == VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16
				|| format == VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16
				|| format == VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM
				|| format == VK_FORMAT_G16_B16R16_2PLANE_420_UNORM)
				size = { 2, 2 };
			else if (format == VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM
				|| format == VK_FORMAT_G8_B8R8_2PLANE_444_UNORM
				|| format == VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16
				|| format == VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16
				|| format == VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16
				|| format == VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16
				|| format == VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM
				|| format == VK_FORMAT_G16_B16R16_2PLANE_444_UNORM)
				size = { 2, 2 };
			else if (format == VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG
				|| format == VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG)
				size = { 8, 4 };

			map[format] = size;
		}
		return map;
	}

	const HashMap<VkFormat, u32> FORMAT_SIZE_MAP = MakeFormatSizeMap();
	const HashMap<VkFormat, Math::Vec2uc> BLOCK_SIZE_MAP = MakeBlockSizeMap();

	u32 VkUtils::GetVkTextureSize(VkFormat format)
	{
		auto size = FORMAT_SIZE_MAP.find(format);
		if (size != FORMAT_SIZE_MAP.end() || size->second == 0) [[unlikely]]
		{
			spdlog::critical("VkFormat not found in the map: {}", magic_enum::enum_name(format));
			exit(-1);
		}
		return size->second;
	}

}
