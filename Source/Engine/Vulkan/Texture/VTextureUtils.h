#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Texture/TextureType.h"
#include "Engine/Graphics/Texture/TextureFormat.h"
#include "Engine/Graphics/Texture/TextureUsageFlags.h"
#include "Engine/Graphics/Texture/SampleCount.h"
#include "Engine/Graphics/Texture/TextureCreateFlags.h"
#include "Engine/Graphics/Texture/TextureViewType.h"
#include "Engine/Graphics/Texture/TextureAspectFlags.h"
#include "Engine/Graphics/Texture/TextureLayout.h"

#include <vulkan/vulkan.h>
#include <magic_enum.hpp>

namespace MAGE
{
	namespace VkUtils
	{
		static VkImageType GetVkImageType(TextureType type)
		{
			switch (type)
			{
			case TextureType::TT_Texture1D:
				return VK_IMAGE_TYPE_1D;
			case TextureType::TT_Texture2D:
				return VK_IMAGE_TYPE_2D;
			case TextureType::TT_Texture3D:
				return VK_IMAGE_TYPE_3D;
			default:
				return VK_IMAGE_TYPE_MAX_ENUM;
			}
		}

		static HashMap<TextureFormat, VkFormat> GetVkFormatMap()
		{
			HashMap<TextureFormat, VkFormat> formatMap;

			for (auto format : magic_enum::enum_values<TextureFormat>())
			{
				if (format == TextureFormat::TF_Unknown)
					formatMap[format] = VK_FORMAT_UNDEFINED;
				else if (format == TextureFormat::TF_RGBA32_Float)
					formatMap[format] = VK_FORMAT_R32G32B32A32_SFLOAT;
				else if (format == TextureFormat::TF_RGBA32_UInt)
					formatMap[format] = VK_FORMAT_R32G32B32A32_UINT;
				else if (format == TextureFormat::TF_RGBA32_Int)
					formatMap[format] = VK_FORMAT_R32G32B32A32_SINT;
				else if (format == TextureFormat::TF_RGB32_Float)
					formatMap[format] = VK_FORMAT_R32G32B32_SFLOAT;
				else if (format == TextureFormat::TF_RGB32_UInt)
					formatMap[format] = VK_FORMAT_R32G32B32_UINT;
				else if (format == TextureFormat::TF_RGB32_Int)
					formatMap[format] = VK_FORMAT_R32G32B32_SINT;
				else if (format == TextureFormat::TF_RGBA16_Float)
					formatMap[format] = VK_FORMAT_R16G16B16A16_SFLOAT;
				else if (format == TextureFormat::TF_RGBA16_UNorm)
					formatMap[format] = VK_FORMAT_R16G16B16A16_UNORM;
				else if (format == TextureFormat::TF_RGBA16_UInt)
					formatMap[format] = VK_FORMAT_R16G16B16A16_UINT;
				else if (format == TextureFormat::TF_RGBA16_Norm)
					formatMap[format] = VK_FORMAT_R16G16B16A16_SNORM;
				else if (format == TextureFormat::TF_RGBA16_Int)
					formatMap[format] = VK_FORMAT_R16G16B16A16_SINT;
				else if (format == TextureFormat::TF_RG32_Float)
					formatMap[format] = VK_FORMAT_R32G32_SFLOAT;
				else if (format == TextureFormat::TF_RG32_UInt)
					formatMap[format] = VK_FORMAT_R32G32_UINT;
				else if (format == TextureFormat::TF_RG32_Int)
					formatMap[format] = VK_FORMAT_R32G32_SINT;
				else if (format == TextureFormat::TF_RGBA8_UNorm)
					formatMap[format] = VK_FORMAT_R8G8B8A8_UNORM;
				else if (format == TextureFormat::TF_RGBA8_UInt)
					formatMap[format] = VK_FORMAT_R8G8B8A8_UINT;
				else if (format == TextureFormat::TF_RGBA8_Norm)
					formatMap[format] = VK_FORMAT_R8G8B8A8_SNORM;
				else if (format == TextureFormat::TF_RGBA8_Int)
					formatMap[format] = VK_FORMAT_R8G8B8A8_SINT;
				else if (format == TextureFormat::TF_RGBA8_SRGB)
					formatMap[format] = VK_FORMAT_R8G8B8A8_SRGB;
				else if (format == TextureFormat::TF_RG16_Float)
					formatMap[format] = VK_FORMAT_R16G16_SFLOAT;
				else if (format == TextureFormat::TF_RG16_UNorm)
					formatMap[format] = VK_FORMAT_R16G16_UNORM;
				else if (format == TextureFormat::TF_RG16_UInt)
					formatMap[format] = VK_FORMAT_R16G16_UINT;
				else if (format == TextureFormat::TF_RG16_Norm)
					formatMap[format] = VK_FORMAT_R16G16_SNORM;
				else if (format == TextureFormat::TF_RG16_Int)
					formatMap[format] = VK_FORMAT_R16G16_SINT;
				else if (format == TextureFormat::TF_D32_Float)
					formatMap[format] = VK_FORMAT_D32_SFLOAT;
				else if (format == TextureFormat::TF_R32_Float)
					formatMap[format] = VK_FORMAT_R32_SFLOAT;
				else if (format == TextureFormat::TF_R32_UInt)
					formatMap[format] = VK_FORMAT_R32_UINT;
				else if (format == TextureFormat::TF_R32_Int)
					formatMap[format] = VK_FORMAT_R32_SINT;
				else if (format == TextureFormat::TF_D24_UNorm_S8_UInt)
					formatMap[format] = VK_FORMAT_D24_UNORM_S8_UINT;
				else if (format == TextureFormat::TF_D32_Float_S8_UInt)
					formatMap[format] = VK_FORMAT_D32_SFLOAT_S8_UINT;
				else if (format == TextureFormat::TF_RG8_UNorm)
					formatMap[format] = VK_FORMAT_R8G8_UNORM;
				else if (format == TextureFormat::TF_RG8_UInt)
					formatMap[format] = VK_FORMAT_R8G8_UINT;
				else if (format == TextureFormat::TF_RG8_Norm)
					formatMap[format] = VK_FORMAT_R8G8_SNORM;
				else if (format == TextureFormat::TF_RG8_Int)
					formatMap[format] = VK_FORMAT_R8G8_SINT;
				else if (format == TextureFormat::TF_R16_Float)
					formatMap[format] = VK_FORMAT_R16_SFLOAT;
				else if (format == TextureFormat::TF_D16_UNorm)
					formatMap[format] = VK_FORMAT_D16_UNORM;
				else if (format == TextureFormat::TF_R16_UNorm)
					formatMap[format] = VK_FORMAT_R16_UNORM;
				else if (format == TextureFormat::TF_R16_UInt)
					formatMap[format] = VK_FORMAT_R16_UINT;
				else if (format == TextureFormat::TF_R16_Norm)
					formatMap[format] = VK_FORMAT_R16_SNORM;
				else if (format == TextureFormat::TF_R16_Int)
					formatMap[format] = VK_FORMAT_R16_SINT;
				else if (format == TextureFormat::TF_R8_UNorm)
					formatMap[format] = VK_FORMAT_R8_UNORM;
				else if (format == TextureFormat::TF_R8_UInt)
					formatMap[format] = VK_FORMAT_R8_UINT;
				else if (format == TextureFormat::TF_R8_Norm)
					formatMap[format] = VK_FORMAT_R8_SNORM;
				else if (format == TextureFormat::TF_R8_Int)
					formatMap[format] = VK_FORMAT_R8_SINT;
			}

			return formatMap;
		}

		const static HashMap<TextureFormat, VkFormat> VkFormatMap = GetVkFormatMap();

		static VkFormat GetVkFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::TF_Unknown:			return VK_FORMAT_UNDEFINED;
			case TextureFormat::TF_RGBA32_Float:		return VK_FORMAT_R32G32B32A32_SFLOAT;
			case TextureFormat::TF_RGBA32_UInt:		return VK_FORMAT_R32G32B32A32_UINT;
			case TextureFormat::TF_RGBA32_Int:			return VK_FORMAT_R32G32B32A32_SINT;

			case TextureFormat::TF_RGB32_Float:		return VK_FORMAT_R32G32B32_SFLOAT;
			case TextureFormat::TF_RGB32_UInt:			return VK_FORMAT_R32G32B32_UINT;
			case TextureFormat::TF_RGB32_Int:			return VK_FORMAT_R32G32B32_SINT;

			case TextureFormat::TF_RGBA16_Float:		return VK_FORMAT_R16G16B16A16_SFLOAT;
			case TextureFormat::TF_RGBA16_UNorm:		return VK_FORMAT_R16G16B16A16_UNORM;
			case TextureFormat::TF_RGBA16_UInt:		return VK_FORMAT_R16G16B16A16_UINT;
			case TextureFormat::TF_RGBA16_Norm:		return VK_FORMAT_R16G16B16A16_SNORM;
			case TextureFormat::TF_RGBA16_Int:			return VK_FORMAT_R16G16B16A16_SINT;

			case TextureFormat::TF_RG32_Float:			return VK_FORMAT_R32G32_SFLOAT;
			case TextureFormat::TF_RG32_UInt:			return VK_FORMAT_R32G32_UINT;
			case TextureFormat::TF_RG32_Int:			return VK_FORMAT_R32G32_SINT;

			case TextureFormat::TF_RGBA8_UNorm:		return VK_FORMAT_R8G8B8A8_UNORM;
			case TextureFormat::TF_RGBA8_UInt:			return VK_FORMAT_R8G8B8A8_UINT;
			case TextureFormat::TF_RGBA8_Norm:			return VK_FORMAT_R8G8B8A8_SNORM;
			case TextureFormat::TF_RGBA8_Int:			return VK_FORMAT_R8G8B8A8_SINT;
			case TextureFormat::TF_RGBA8_SRGB:			return VK_FORMAT_R8G8B8A8_SRGB;

			case TextureFormat::TF_RG16_Float:			return VK_FORMAT_R16G16_SFLOAT;
			case TextureFormat::TF_RG16_UNorm:			return VK_FORMAT_R16G16_UNORM;
			case TextureFormat::TF_RG16_UInt:			return VK_FORMAT_R16G16_UINT;
			case TextureFormat::TF_RG16_Norm:			return VK_FORMAT_R16G16_SNORM;
			case TextureFormat::TF_RG16_Int:			return VK_FORMAT_R16G16_SINT;

			case TextureFormat::TF_D32_Float:			return VK_FORMAT_D32_SFLOAT;
			case TextureFormat::TF_R32_Float:			return VK_FORMAT_R32_SFLOAT;
			case TextureFormat::TF_R32_UInt:			return VK_FORMAT_R32_UINT;
			case TextureFormat::TF_R32_Int:			return VK_FORMAT_R32_SINT;

			case TextureFormat::TF_D24_UNorm_S8_UInt:	return VK_FORMAT_D24_UNORM_S8_UINT;
			case TextureFormat::TF_D32_Float_S8_UInt:	return VK_FORMAT_D32_SFLOAT_S8_UINT;

			case TextureFormat::TF_RG8_UNorm:			return VK_FORMAT_R8G8_UNORM;
			case TextureFormat::TF_RG8_UInt:			return VK_FORMAT_R8G8_UINT;
			case TextureFormat::TF_RG8_Norm:			return VK_FORMAT_R8G8_SNORM;
			case TextureFormat::TF_RG8_Int:			return VK_FORMAT_R8G8_SINT;

			case TextureFormat::TF_R16_Float:			return VK_FORMAT_R16_SFLOAT;
			case TextureFormat::TF_D16_UNorm:			return VK_FORMAT_D16_UNORM;
			case TextureFormat::TF_R16_UNorm:			return VK_FORMAT_R16_UNORM;
			case TextureFormat::TF_R16_UInt:			return VK_FORMAT_R16_UINT;
			case TextureFormat::TF_R16_Norm:			return VK_FORMAT_R16_SNORM;
			case TextureFormat::TF_R16_Int:			return VK_FORMAT_R16_SINT;

			case TextureFormat::TF_R8_UNorm:			return VK_FORMAT_R8_UNORM;
			case TextureFormat::TF_R8_UInt:			return VK_FORMAT_R8_UINT;
			case TextureFormat::TF_R8_Norm:			return VK_FORMAT_R8_SNORM;
			case TextureFormat::TF_R8_Int:				return VK_FORMAT_R8_SINT;

			default:								return VK_FORMAT_UNDEFINED;
			}
		}

		static VkImageUsageFlags GetVkImageUsageFlags(TextureUsageFlags flags)
		{
			VkImageUsageFlags vkFlags = 0;

			if (flags & TextureUsageFlags::TUF_SampledImage)
				vkFlags |= VK_IMAGE_USAGE_SAMPLED_BIT;

			if (flags & TextureUsageFlags::TUF_ColorAttachment)
				vkFlags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			else if (flags & TextureUsageFlags::TUF_DepthStencilAttachment)
				vkFlags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

			if (flags & TextureUsageFlags::TUF_StorageImage)
				vkFlags |= VK_IMAGE_USAGE_STORAGE_BIT;
			if (flags & TextureUsageFlags::TUF_TransferDestination)
				vkFlags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			else if (flags & TextureUsageFlags::TUF_TransferSource)
				vkFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

			return vkFlags;
		}

		static VkSampleCountFlagBits GetVkSampleCountFlags(SampleCount count)
		{
			switch (count)
			{
			case SampleCount::SC_Sample1:	return VK_SAMPLE_COUNT_1_BIT;
			case SampleCount::SC_Sample2:	return VK_SAMPLE_COUNT_2_BIT;
			case SampleCount::SC_Sample4:	return VK_SAMPLE_COUNT_4_BIT;
			case SampleCount::SC_Sample8:	return VK_SAMPLE_COUNT_8_BIT;
			case SampleCount::SC_Sample16:	return VK_SAMPLE_COUNT_16_BIT;
			case SampleCount::SC_Sample32:	return VK_SAMPLE_COUNT_32_BIT;
			case SampleCount::SC_Sample64:	return VK_SAMPLE_COUNT_64_BIT;
			default:						return VK_SAMPLE_COUNT_1_BIT;
			}
		}

		static VkImageCreateFlags GetVkImageCreateFlags(TextureCreateFlags flags)
		{
			VkImageCreateFlags vkFlags = VkImageCreateFlags();

			if (flags & TextureCreateFlags::TCF_Undefined)
				return 0;

			if (flags & TextureCreateFlags::TCF_SparseBinding)
				vkFlags |= VK_IMAGE_CREATE_SPARSE_BINDING_BIT;
			if (flags & TextureCreateFlags::TCF_SparseResidency)
				vkFlags |= VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT;
			if (flags & TextureCreateFlags::TCF_SparseAliased)
				vkFlags |= VK_IMAGE_CREATE_SPARSE_ALIASED_BIT;
			if (flags & TextureCreateFlags::TCF_MutableFormat)
				vkFlags |= VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT;
			if (flags & TextureCreateFlags::TCF_CubeCompatible)
				vkFlags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
			if (flags & TextureCreateFlags::TCF_Array2D)
				vkFlags |= VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT;
			if (flags & TextureCreateFlags::TCF_SplitInstanceBindRegions)
				vkFlags |= VK_IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT;
			if (flags & TextureCreateFlags::TCF_BlockTexelView)
				vkFlags |= VK_IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT;
			if (flags & TextureCreateFlags::TCF_ExtendedUsage)
				vkFlags |= VK_IMAGE_CREATE_EXTENDED_USAGE_BIT;
			if (flags & TextureCreateFlags::TCF_Disjoint)
				vkFlags |= VK_IMAGE_CREATE_DISJOINT_BIT;
			if (flags & TextureCreateFlags::TCF_Alias)
				vkFlags |= VK_IMAGE_CREATE_ALIAS_BIT;
			if (flags & TextureCreateFlags::TCF_Protected)
				vkFlags |= VK_IMAGE_CREATE_PROTECTED_BIT;

			return vkFlags;
		}

		static VkImageViewType GetVkTextureViewType(TextureViewType type)
		{
			switch (type)
			{
			case TextureViewType::TVT_Texture1D:		return VK_IMAGE_VIEW_TYPE_1D;
			case TextureViewType::TVT_Texture2D:		return VK_IMAGE_VIEW_TYPE_2D;
			case TextureViewType::TVT_Texture3D:		return VK_IMAGE_VIEW_TYPE_3D;
			case TextureViewType::TVT_TextureCube:		return VK_IMAGE_VIEW_TYPE_CUBE;
			case TextureViewType::TVT_Texture1DArray:	return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
			case TextureViewType::TVT_Texture2DArray:	return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
			case TextureViewType::TVT_TextureCubeArray:	return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
			default:									return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
			}
		}

		static VkImageAspectFlags GetVkTextureAspectFlags(TextureAspectFlags flags)
		{
			VkImageAspectFlags vkFlags = 0;

			if (flags & TextureAspectFlags::TAF_Undefined)
				return 0;

			if (flags & TextureAspectFlags::TAF_ColorAspect)
				vkFlags |= VK_IMAGE_ASPECT_COLOR_BIT;
			if (flags & TextureAspectFlags::TAF_DepthAspect)
				vkFlags |= VK_IMAGE_ASPECT_DEPTH_BIT;
			if (flags & TextureAspectFlags::TAF_StencilAspect)
				vkFlags |= VK_IMAGE_ASPECT_STENCIL_BIT;
			if (flags & TextureAspectFlags::TAF_MetaAspect)
				vkFlags |= VK_IMAGE_ASPECT_METADATA_BIT;

			return vkFlags;
		}

		static VkImageLayout GetVkImageLayout(TextureLayout layout)
		{
			switch (layout)
			{
			case TextureLayout::TL_Undefined:				return VK_IMAGE_LAYOUT_UNDEFINED;
			case TextureLayout::TL_General:				return VK_IMAGE_LAYOUT_GENERAL;
			case TextureLayout::TL_ColorAttachment:		return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			case TextureLayout::TL_DepthStencilAttachment: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			case TextureLayout::TL_ShaderReadOnly:			return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			case TextureLayout::TL_TransferSource:			return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			case TextureLayout::TL_TransferDestination:	return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			case TextureLayout::TL_Present:				return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			default:									return VK_IMAGE_LAYOUT_UNDEFINED;
			}
		}
	}
}
