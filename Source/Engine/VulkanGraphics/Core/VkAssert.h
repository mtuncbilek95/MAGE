/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/ErrorHandler/ErrorChecker.h"

#include <magic_enum.hpp>
#include <vulkan/vulkan.h>

namespace MAGE
{
	namespace ErrorUtils
	{
		inline void VkAssert(VkResult result, const String& title)
		{
			if (result != VK_SUCCESS)
			{
				auto name = magic_enum::enum_name<VkResult>(result);
				spdlog::critical("{0} - {1}", title, name);
				exit(-1);
			}
		}
	}
}