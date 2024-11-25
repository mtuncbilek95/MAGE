/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	struct Vertex final
	{
		Math::Vec3f position;
		Math::Vec3f normal;
		Math::Vec3f tangent;
		Math::Vec3f bitangent;
		Math::Vec4f color;
		Math::Vec2f uv;
	};

	struct BoneWeight final
	{

	};

	struct Geometry final
	{
		vector<Vertex> vertices;
		vector<u32> indices;
	};
}
