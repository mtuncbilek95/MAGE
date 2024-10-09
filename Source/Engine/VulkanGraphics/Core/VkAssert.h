#pragma once

#include "Engine/Core/Core.h"
#include "Engine/ErrorHandler/ErrorChecker.h"

#include <magic_enum.hpp>
#include <vulkan/vulkan.hpp>

namespace MAGE
{
	namespace ErrorUtils
	{
		void VkAssert(VkResult result, const String& title)
		{
			if (result != VK_SUCCESS)
			{
				auto name = magic_enum::enum_name<VkResult>(result);
				spdlog::critical("{0} - {1}", title, name);
			}
		}
	}
}