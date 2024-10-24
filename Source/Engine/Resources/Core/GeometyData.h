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
	struct GeometryData final
	{
		Vector<Math::Vec3f> position;
		Vector<Math::Vec3f> normal;
		Vector<Math::Vec3f> tangent;
		Vector<Math::Vec3f> bitangent;
		Vector<Math::Vec2f> uv;
		Vector<Math::Vec4f> color;

		Vector<u32> indices;
	};
}
