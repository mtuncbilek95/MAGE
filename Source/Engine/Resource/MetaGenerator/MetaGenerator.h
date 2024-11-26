/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace MAGE
{
	class Resource;

	class MetaHandler final
	{
	public:
		static json GetResourceMeta(const path& srcPath, const path& dstPath);
	};

	class ResourceHandler final
	{
	public:
		static Shared<Resource> GetResourceData(const path& srcPath, const path& dstPath);
	};
}
