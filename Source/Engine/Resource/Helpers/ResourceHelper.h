/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Memory/OwnedBuffer.h"
#include "Engine/Resource/Data/Geometry.h"
#include "Engine/Resource/Data/Imagery.h"
#include "Engine/Resource/Core/Resource.h"

namespace MAGE
{
	class ResourceHandler
	{
	public:
		static void ReadFileFromDisk(const string& aPath, const string& relExtPath, ResourceProps& props);
		static void ReadResourceFile(const string& fPath, OwnedBuffer& buffer);
		static void ReadGeometryDisk(const string& fPath, Geometry& buffer);
		static void ReadTextureDisk(const string& fPath, Imagery& buffer);
	};
}
