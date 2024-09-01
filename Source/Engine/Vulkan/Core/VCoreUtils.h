#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Core/CompareOperation.h"
#include "Engine/Graphics/Core/LogicOperation.h"

#include <vulkan/vulkan.h>

namespace MAGE
{
	namespace VkUtils
	{
		static VkCompareOp GetVkCompareOp(CompareOperation operation)
		{
			switch (operation)
			{
			case CompareOperation::CO_Never:
				return VK_COMPARE_OP_NEVER;
			case CompareOperation::CO_Less:
				return VK_COMPARE_OP_LESS;
			case CompareOperation::CO_Equal:
				return VK_COMPARE_OP_EQUAL;
			case CompareOperation::CO_LessEqual:
				return VK_COMPARE_OP_LESS_OR_EQUAL;
			case CompareOperation::CO_Greater:
				return VK_COMPARE_OP_GREATER;
			case CompareOperation::CO_NotEqual:
				return VK_COMPARE_OP_NOT_EQUAL;
			case CompareOperation::CO_GreaterEqual:
				return VK_COMPARE_OP_GREATER_OR_EQUAL;
			case CompareOperation::CO_Always:
				return VK_COMPARE_OP_ALWAYS;
			default:
				return VK_COMPARE_OP_NEVER;
			}
		}

		static VkLogicOp GetVkLogicOperation(LogicOperation operation)
		{
			switch (operation)
			{
			case LogicOperation::LO_Clear:
				return VK_LOGIC_OP_CLEAR;
			case LogicOperation::LO_And:
				return VK_LOGIC_OP_AND;
			case LogicOperation::LO_AndReverse:
				return VK_LOGIC_OP_AND_REVERSE;
			case LogicOperation::LO_Copy:
				return VK_LOGIC_OP_COPY;
			case LogicOperation::LO_AndInverted:
				return VK_LOGIC_OP_AND_INVERTED;
			case LogicOperation::LO_NoOp:
				return VK_LOGIC_OP_NO_OP;
			case LogicOperation::LO_Xor:
				return VK_LOGIC_OP_XOR;
			case LogicOperation::LO_Or:
				return VK_LOGIC_OP_OR;
			case LogicOperation::LO_Nor:
				return VK_LOGIC_OP_NOR;
			case LogicOperation::LO_Equal:
				return VK_LOGIC_OP_EQUIVALENT;
			case LogicOperation::LO_Invert:
				return VK_LOGIC_OP_INVERT;
			case LogicOperation::LO_OrReverse:
				return VK_LOGIC_OP_OR_REVERSE;
			case LogicOperation::LO_CopyInverted:
				return VK_LOGIC_OP_COPY_INVERTED;
			case LogicOperation::LO_OrInverted:
				return VK_LOGIC_OP_OR_INVERTED;
			case LogicOperation::LO_NotAnd:
				return VK_LOGIC_OP_NAND;
			case LogicOperation::LO_Set:
				return VK_LOGIC_OP_SET;
			default:
				return VK_LOGIC_OP_CLEAR;
			}
		}
	}
}
