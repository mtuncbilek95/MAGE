#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Shader/ShaderStage.h"

#include <vulkan/vulkan.h>

namespace MAGE
{
	namespace VkUtils
	{
		static VkShaderStageFlags GetVkShaderStage(ShaderStage stage)
		{
			VkShaderStageFlags vkStage = 0;

			if (stage & ShaderStage::SS_Vertex)
				vkStage |= VK_SHADER_STAGE_VERTEX_BIT;
			if (stage & ShaderStage::SS_TessellationControl)
				vkStage |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
			if (stage & ShaderStage::SS_TessellationEvaluation)
				vkStage |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
			if (stage & ShaderStage::SS_Geometry)
				vkStage |= VK_SHADER_STAGE_GEOMETRY_BIT;
			if (stage & ShaderStage::SS_Fragment)
				vkStage |= VK_SHADER_STAGE_FRAGMENT_BIT;
			if (stage & ShaderStage::SS_Compute)
				vkStage |= VK_SHADER_STAGE_COMPUTE_BIT;

			return vkStage;
		}

		static VkShaderStageFlagBits GetVkShaderStageBit(ShaderStage stage)
		{
			switch (stage)
			{
			case ShaderStage::SS_Vertex:
				return VK_SHADER_STAGE_VERTEX_BIT;
			case ShaderStage::SS_TessellationControl:
				return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
			case ShaderStage::SS_TessellationEvaluation:
				return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
			case ShaderStage::SS_Geometry:
				return VK_SHADER_STAGE_GEOMETRY_BIT;
			case ShaderStage::SS_Fragment:
				return VK_SHADER_STAGE_FRAGMENT_BIT;
			case ShaderStage::SS_Compute:
				return VK_SHADER_STAGE_COMPUTE_BIT;
			default:
				return VK_SHADER_STAGE_VERTEX_BIT;
			}
		}
	}
}