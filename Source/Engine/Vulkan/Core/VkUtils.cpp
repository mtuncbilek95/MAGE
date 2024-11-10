#include "VkUtils.h"

#include <magic_enum.hpp>

namespace MAGE
{
	HashMap<vk::Format, u32> MakeFormatSizeMap()
	{
		HashMap<vk::Format, u32> map;
		for (auto format : magic_enum::enum_values<vk::Format>())
		{
			u32 size = 0;
			if (format == vk::Format::eR4G4UnormPack8 || format == vk::Format::eS8Uint) size = 1;
			else if (format >= vk::Format::eR4G4B4A4UnormPack16 && format <= vk::Format::eA1R5G5B5UnormPack16)
				size = 2;
			else if (format >= vk::Format::eR8Unorm && format <= vk::Format::eR8Srgb) size = 1;
			else if (format >= vk::Format::eR8G8Unorm && format <= vk::Format::eR8G8Srgb) size = 2;
			else if (format >= vk::Format::eR8G8B8Unorm && format <= vk::Format::eB8G8R8Srgb) size = 3;
			else if (format >= vk::Format::eR8G8B8A8Unorm && format <= vk::Format::eA2B10G10R10SintPack32)
				size = 4;
			else if (format >= vk::Format::eR16Unorm && format <= vk::Format::eR16Sfloat) size = 2;
			else if (format >= vk::Format::eR16G16Unorm && format <= vk::Format::eR16G16Sfloat) size = 4;
			else if (format >= vk::Format::eR16G16B16Unorm && format <= vk::Format::eR16G16B16Sfloat) size = 6;
			else if (format >= vk::Format::eR16G16B16A16Unorm && format <= vk::Format::eR16G16B16A16Sfloat)
				size = 8;
			else if (format >= vk::Format::eR32Uint && format <= vk::Format::eR32Sfloat) size = 4;
			else if (format >= vk::Format::eR32G32Uint && format <= vk::Format::eR32G32Sfloat) size = 8;
			else if (format >= vk::Format::eR32G32B32Uint && format <= vk::Format::eR32G32B32Sfloat) size = 12;
			else if (format >= vk::Format::eR32G32B32A32Uint && format <= vk::Format::eR32G32B32A32Sfloat)
				size = 16;
			else if (format >= vk::Format::eR64Uint && format <= vk::Format::eR64Sfloat) size = 8;
			else if (format >= vk::Format::eR64G64Uint && format <= vk::Format::eR64G64Sfloat) size = 16;
			else if (format >= vk::Format::eR64G64B64Uint && format <= vk::Format::eR64G64B64Sfloat) size = 24;
			else if (format >= vk::Format::eR64G64B64A64Uint && format <= vk::Format::eR64G64B64A64Sfloat)
				size = 32;
			else if (format == vk::Format::eB10G11R11UfloatPack32 || format == vk::Format::eE5B9G9R9UfloatPack32
				|| format == vk::Format::eX8D24UnormPack32 || format == vk::Format::eD32Sfloat
				|| format == vk::Format::eD24UnormS8Uint)
				size = 4;
			else if (format == vk::Format::eD16Unorm) size = 2;
			else if (format == vk::Format::eD16UnormS8Uint) size = 3;
			else if (format == vk::Format::eD32SfloatS8Uint) size = 5;
			else if (format == vk::Format::eBc1RgbUnormBlock || format == vk::Format::eBc1RgbSrgbBlock
				|| format == vk::Format::eBc1RgbaUnormBlock || format == vk::Format::eBc1RgbaSrgbBlock
				|| format == vk::Format::eBc4SnormBlock || format == vk::Format::eBc4UnormBlock)
				size = 8;
			else if (format == vk::Format::eBc2SrgbBlock || format == vk::Format::eBc2UnormBlock
				|| format == vk::Format::eBc3SrgbBlock || format == vk::Format::eBc3UnormBlock
				|| format == vk::Format::eBc5SnormBlock || format == vk::Format::eBc5UnormBlock
				|| format == vk::Format::eBc6HSfloatBlock || format == vk::Format::eBc6HUfloatBlock
				|| format == vk::Format::eBc7SrgbBlock || format == vk::Format::eBc7UnormBlock)
				size = 16;
			else if (format == vk::Format::eEtc2R8G8B8UnormBlock || format == vk::Format::eEtc2R8G8B8SrgbBlock
				|| format == vk::Format::eEtc2R8G8B8A1UnormBlock
				|| format == vk::Format::eEtc2R8G8B8A1SrgbBlock
				|| format == vk::Format::eEtc2R8G8B8A8UnormBlock
				|| format == vk::Format::eEtc2R8G8B8A8SrgbBlock || format == vk::Format::eEacR11UnormBlock
				|| format == vk::Format::eEacR11SnormBlock || format == vk::Format::eEacR11G11UnormBlock
				|| format == vk::Format::eEacR11G11SnormBlock)
				size = 16;
			else if (format == vk::Format::eAstc4x4UnormBlock || format == vk::Format::eAstc4x4SfloatBlock
				|| format == vk::Format::eAstc4x4SrgbBlock || format == vk::Format::eAstc5x4UnormBlock
				|| format == vk::Format::eAstc5x4SfloatBlock || format == vk::Format::eAstc5x4SrgbBlock
				|| format == vk::Format::eAstc5x5UnormBlock || format == vk::Format::eAstc5x5SfloatBlock
				|| format == vk::Format::eAstc5x5SrgbBlock || format == vk::Format::eAstc6x5UnormBlock
				|| format == vk::Format::eAstc6x5SfloatBlock || format == vk::Format::eAstc6x5SrgbBlock
				|| format == vk::Format::eAstc6x6UnormBlock || format == vk::Format::eAstc6x6SfloatBlock
				|| format == vk::Format::eAstc6x6SrgbBlock || format == vk::Format::eAstc8x5UnormBlock
				|| format == vk::Format::eAstc8x5SfloatBlock || format == vk::Format::eAstc8x5SrgbBlock
				|| format == vk::Format::eAstc8x6UnormBlock || format == vk::Format::eAstc8x6SfloatBlock
				|| format == vk::Format::eAstc8x6SrgbBlock || format == vk::Format::eAstc8x8UnormBlock
				|| format == vk::Format::eAstc8x8SfloatBlock || format == vk::Format::eAstc8x8SrgbBlock
				|| format == vk::Format::eAstc10x5UnormBlock || format == vk::Format::eAstc10x5SfloatBlock
				|| format == vk::Format::eAstc10x5SrgbBlock || format == vk::Format::eAstc10x6UnormBlock
				|| format == vk::Format::eAstc10x6SfloatBlock || format == vk::Format::eAstc10x6SrgbBlock
				|| format == vk::Format::eAstc10x8UnormBlock || format == vk::Format::eAstc10x8SfloatBlock
				|| format == vk::Format::eAstc10x8SrgbBlock || format == vk::Format::eAstc10x10UnormBlock
				|| format == vk::Format::eAstc10x10SfloatBlock || format == vk::Format::eAstc10x10SrgbBlock
				|| format == vk::Format::eAstc12x10UnormBlock
				|| format == vk::Format::eAstc12x10SfloatBlock || format == vk::Format::eAstc12x10SrgbBlock
				|| format == vk::Format::eAstc12x12UnormBlock
				|| format == vk::Format::eAstc12x12SfloatBlock || format == vk::Format::eAstc12x12SrgbBlock)
				size = 16;

			map[format] = size;
		}

		return map;
	}

	const HashMap<vk::Format, u32> FORMAT_SIZE_MAP = MakeFormatSizeMap();

	u32 VkUtils::GetVkTextureSize(vk::Format format)
	{
		auto size = FORMAT_SIZE_MAP.find(format);
		if (size == FORMAT_SIZE_MAP.end() || size->second == 0) [[unlikely]]
		{
			spdlog::critical("VkFormat not found in the map: {}", magic_enum::enum_name(format));
			exit(-1);
		}
		return size->second;
	}
}
