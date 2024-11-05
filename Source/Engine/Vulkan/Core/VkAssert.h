/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"

#include <vulkan/vulkan.hpp>
#include <magic_enum.hpp>

namespace MAGE
{
	namespace ErrorUtils
	{
		static inline void VkAssert(vk::Result result, const String& objectTitle)
		{
			if (result != vk::Result::eSuccess)
			{
				auto name = magic_enum::enum_name(result);
				spdlog::critical("{0} - {1}", std::string_view(objectTitle), name);
				exit(-1);
			}
		}
	}
}