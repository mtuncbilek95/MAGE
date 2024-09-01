#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Queue/GraphicsQueueType.h"
#include "Engine/Graphics/Command/CmdPoolType.h"
#include "Engine/Graphics/Command/Core/GraphicsIndexType.h"
#include "Engine/Graphics/Command/Core/ResolveModeFlags.h"
#include "Engine/Graphics/Command/Core/AttachmentLoadOperation.h"
#include "Engine/Graphics/Command/Core/AttachmentStoreOperation.h"

#include <vulkan/vulkan.h>

namespace MAGE
{
	namespace VkUtils
	{
		static GraphicsQueueType GetQueueType(CmdPoolType type)
		{
			switch (type)
			{
			case CmdPoolType::CPT_Graphics:
				return GraphicsQueueType::GQT_Graphics;
			case CmdPoolType::CPT_Compute:
				return GraphicsQueueType::GQT_Compute;
			case CmdPoolType::CPT_Transfer:
				return GraphicsQueueType::GQT_Transfer;
			default:
				return GraphicsQueueType::GQT_Graphics;
			}
		}

		static VkIndexType GetIndexType(GraphicsIndexType type)
		{
			switch (type)
			{
			case GraphicsIndexType::GIT_Index16:
				return VK_INDEX_TYPE_UINT16;
			case GraphicsIndexType::GIT_Index32:
				return VK_INDEX_TYPE_UINT32;
			default:
				return VK_INDEX_TYPE_UINT16;
			}
		}

		static VkResolveModeFlagBits GetResolveMode(ResolveModeFlags flags)
		{
			switch (flags)
			{
			case ResolveModeFlags::RMF_None:
				return VK_RESOLVE_MODE_NONE;
			case ResolveModeFlags::RMF_SampleZero:
				return VK_RESOLVE_MODE_SAMPLE_ZERO_BIT;
			case ResolveModeFlags::RMF_Average:
				return VK_RESOLVE_MODE_AVERAGE_BIT;
			case ResolveModeFlags::RMF_Min:
				return VK_RESOLVE_MODE_MIN_BIT;
			case ResolveModeFlags::RMF_Max:
				return VK_RESOLVE_MODE_MAX_BIT;
			default:
				return VK_RESOLVE_MODE_NONE;
			}
		}

		static VkAttachmentLoadOp GetVkAttachmentLoadOp(AttachmentLoadOperation op)
		{
			switch (op)
			{
			case AttachmentLoadOperation::ALO_Load:
				return VK_ATTACHMENT_LOAD_OP_LOAD;
			case AttachmentLoadOperation::ALO_Clear:
				return VK_ATTACHMENT_LOAD_OP_CLEAR;
			case AttachmentLoadOperation::ALO_DontCare:
				return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			default:
				return VK_ATTACHMENT_LOAD_OP_LOAD;
			}
		}

		static VkAttachmentStoreOp GetVkAttachmentStoreOp(AttachmentStoreOperation op)
		{
			switch (op)
			{
			case AttachmentStoreOperation::ASO_Store:
				return VK_ATTACHMENT_STORE_OP_STORE;
			case AttachmentStoreOperation::ASO_DontCare:
				return VK_ATTACHMENT_STORE_OP_DONT_CARE;
			default:
				return VK_ATTACHMENT_STORE_OP_STORE;
			}
		}
	}
}