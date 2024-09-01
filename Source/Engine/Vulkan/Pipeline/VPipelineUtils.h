#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Vulkan/Core/VCoreUtils.h"
#include "Engine/Graphics/Pipeline/InputLayout/InputBindingStepRate.h"
#include "Engine/Graphics/Pipeline/InputLayout/MeshTopology.h"
#include "Engine/Graphics/Pipeline/RasterizerState/PolygonMode.h"
#include "Engine/Graphics/Pipeline/RasterizerState/FaceCullMode.h"
#include "Engine/Graphics/Pipeline/BlendState/BlendFactor.h"
#include "Engine/Graphics/Pipeline/BlendState/BlendOperation.h"
#include "Engine/Graphics/Pipeline/BlendState/BlendColorWriteMask.h"
#include "Engine/Graphics/Pipeline/DepthStencilState/StencilOperation.h"
#include "Engine/Graphics/Pipeline/PipelineBindPoint.h"
#include "Engine/Graphics/Pipeline/PipelineStageFlags.h"
#include "Engine/Graphics/Pipeline/DepthStencilState/StencilFaceDesc.h"

#include <vulkan/vulkan.h>

namespace MAGE
{
	namespace VkUtils
	{
		static VkPrimitiveTopology GetVkPrimitiveTopology(MeshTopology topology)
		{
			switch (topology)
			{
			case MeshTopology::MT_PointList: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			case MeshTopology::MT_LineList: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			case MeshTopology::MT_LineStrip: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			case MeshTopology::MT_TriangleList: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			case MeshTopology::MT_PatchList: return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
			case MeshTopology::MT_TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			case MeshTopology::MT_TriangleFan: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
			default: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			}
		}

		static VkPolygonMode GetVkPolygonMode(PolygonMode mode)
		{
			switch (mode)
			{
			case PolygonMode::PM_Fill: return VK_POLYGON_MODE_FILL;
			case PolygonMode::PM_Line: return VK_POLYGON_MODE_LINE;
			case PolygonMode::PM_Point: return VK_POLYGON_MODE_POINT;
			default: return VK_POLYGON_MODE_FILL;
			}
		}

		static VkCullModeFlags GetVkCullMode(FaceCullMode mode)
		{
			switch (mode)
			{
			case FaceCullMode::FCM_None: return VK_CULL_MODE_NONE;
			case FaceCullMode::FCM_Front: return VK_CULL_MODE_FRONT_BIT;
			case FaceCullMode::FCM_Back: return VK_CULL_MODE_BACK_BIT;
			case FaceCullMode::FCM_All: return VK_CULL_MODE_FRONT_AND_BACK;
			default: return VK_CULL_MODE_NONE;
			}
		}

		static VkBlendFactor GetVkBlendFactor(BlendFactor factor)
		{
			switch (factor)
			{
			case BlendFactor::BF_Zero: return VK_BLEND_FACTOR_ZERO;
			case BlendFactor::BF_One: return VK_BLEND_FACTOR_ONE;
			case BlendFactor::BF_SrcColor: return VK_BLEND_FACTOR_SRC_COLOR;
			case BlendFactor::BF_OneMinusSrcColor: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
			case BlendFactor::BF_DstColor: return VK_BLEND_FACTOR_DST_COLOR;
			case BlendFactor::BF_OneMinusDstColor: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			case BlendFactor::BF_SrcAlpha: return VK_BLEND_FACTOR_SRC_ALPHA;
			case BlendFactor::BF_OneMinusSrcAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			case BlendFactor::BF_DstAlpha: return VK_BLEND_FACTOR_DST_ALPHA;
			case BlendFactor::BF_OneMinusDstAlpha: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			case BlendFactor::BF_ConstantColor: return VK_BLEND_FACTOR_CONSTANT_COLOR;
			case BlendFactor::BF_OneMinusConstantColor: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
			case BlendFactor::BF_ConstantAlpha: return VK_BLEND_FACTOR_CONSTANT_ALPHA;
			case BlendFactor::BF_OneMinusConstantAlpha: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
			case BlendFactor::BF_SrcAlphaSat: return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
			case BlendFactor::BF_Src1Color: return VK_BLEND_FACTOR_SRC1_COLOR;
			case BlendFactor::BF_OneMinusSrc1Color: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
			case BlendFactor::BF_Src1Alpha: return VK_BLEND_FACTOR_SRC1_ALPHA;
			case BlendFactor::BF_OneMinusSrc1Alpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
			default: return VK_BLEND_FACTOR_ONE;
			}
		}

		static VkBlendOp GetVkBlendOperation(BlendOperation operation)
		{
			switch (operation)
			{
			case BlendOperation::BO_Add: return VK_BLEND_OP_ADD;
			case BlendOperation::BO_Subtract: return VK_BLEND_OP_SUBTRACT;
			case BlendOperation::BO_ReverseSubtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
			case BlendOperation::BO_Min: return VK_BLEND_OP_MIN;
			case BlendOperation::BO_Max: return VK_BLEND_OP_MAX;
			default: return VK_BLEND_OP_ADD;
			}
		}

		static VkColorComponentFlags GetVkColorWriteMask(BlendColorWriteMask mask)
		{
			VkColorComponentFlags flags = 0;

			if (mask & BlendColorWriteMask::CWM_Red)
				flags |= VK_COLOR_COMPONENT_R_BIT;
			if (mask & BlendColorWriteMask::CWM_Green)
				flags |= VK_COLOR_COMPONENT_G_BIT;
			if (mask & BlendColorWriteMask::CWM_Blue)
				flags |= VK_COLOR_COMPONENT_B_BIT;
			if (mask & BlendColorWriteMask::CWM_Alpha)
				flags |= VK_COLOR_COMPONENT_A_BIT;

			return flags;
		}

		static VkVertexInputRate GetVkVertexInputRate(InputBindingStepRate rate)
		{
			switch (rate)
			{
			case InputBindingStepRate::IBS_Vertex: return VK_VERTEX_INPUT_RATE_VERTEX;
			case InputBindingStepRate::IBS_Instance: return VK_VERTEX_INPUT_RATE_INSTANCE;
			default: return VK_VERTEX_INPUT_RATE_VERTEX;
			}
		}

		static VkPipelineBindPoint GetVkPipelineBindPoint(PipelineBindPoint point)
		{
			switch (point)
			{
			case PipelineBindPoint::PBP_Graphics: return VK_PIPELINE_BIND_POINT_GRAPHICS;
			case PipelineBindPoint::PBP_Compute: return VK_PIPELINE_BIND_POINT_COMPUTE;
			default: return VK_PIPELINE_BIND_POINT_GRAPHICS;
			}
		}

		static VkPipelineStageFlags GetVkPipelineStageFlags(PipelineStageFlags flags)
		{
			VkPipelineStageFlags result = 0;

			if (flags & PipelineStageFlags::PSF_TopOfPipe)
				result |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			if (flags & PipelineStageFlags::PSF_VertexInput)
				result |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
			if (flags & PipelineStageFlags::PSF_VertexShader)
				result |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
			if (flags & PipelineStageFlags::PSF_TessellationControlShader)
				result |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
			if (flags & PipelineStageFlags::PSF_TessellationEvaluationShader)
				result |= VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
			if (flags & PipelineStageFlags::PSF_GeometryShader)
				result |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
			if (flags & PipelineStageFlags::PSF_FragmentShader)
				result |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			if (flags & PipelineStageFlags::PSF_EarlyFragmentTests)
				result |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			if (flags & PipelineStageFlags::PSF_LateFragmentTests)
				result |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			if (flags & PipelineStageFlags::PSF_ColorAttachmentOutput)
				result |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			if (flags & PipelineStageFlags::PSF_ComputeShader)
				result |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
			if (flags & PipelineStageFlags::PSF_Transfer)
				result |= VK_PIPELINE_STAGE_TRANSFER_BIT;
			if (flags & PipelineStageFlags::PSF_BottomOfPipe)
				result |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			if (flags & PipelineStageFlags::PSF_Host)
				result |= VK_PIPELINE_STAGE_HOST_BIT;
			if (flags & PipelineStageFlags::PSF_AllGraphics)
				result |= VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
			if (flags & PipelineStageFlags::PSF_AllCommands)
				result |= VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

			return result;

		}

		static VkStencilOp GetVkStencilOp(StencilOperation operation)
		{
			switch (operation)
			{
			case StencilOperation::SO_Keep: return VK_STENCIL_OP_KEEP;
			case StencilOperation::SO_Zero: return VK_STENCIL_OP_ZERO;
			case StencilOperation::SO_Replace: return VK_STENCIL_OP_REPLACE;
			case StencilOperation::SO_IncrementAndClamp: return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
			case StencilOperation::SO_DecrementAndClamp: return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
			case StencilOperation::SO_Invert: return VK_STENCIL_OP_INVERT;
			case StencilOperation::SO_IncrementAndWrap: return VK_STENCIL_OP_INCREMENT_AND_WRAP;
			case StencilOperation::SO_DecrementAndWrap: return VK_STENCIL_OP_DECREMENT_AND_WRAP;
			default: return VK_STENCIL_OP_KEEP;
			}
		}

		static VkStencilOpState GetVkStencilOpState(StencilFaceDesc desc)
		{
			VkStencilOpState state{};
			state.failOp = GetVkStencilOp(desc.FailOperation);
			state.passOp = GetVkStencilOp(desc.PassOperation);
			state.depthFailOp = GetVkStencilOp(desc.DepthFailOperation);
			state.compareOp = GetVkCompareOp(desc.CompareOperation);
			state.compareMask = desc.CompareMask;
			state.writeMask = desc.WriteMask;
			state.reference = desc.Reference;
			return state;
		}
	}
}
