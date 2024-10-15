/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include <vulkan/vulkan.h>

namespace MAGE
{
	struct DepthStencilState final
	{
		b8 depthTestEnable;
		b8 depthWriteEnable;
		VkCompareOp depthCompareOp;
		b8 depthBoundsTestEnable;
		b8 stencilTestEnable;
		VkStencilOpState front;
		VkStencilOpState back;
		f32 minDepthBounds;
		f32 maxDepthBounds;
	};
}
