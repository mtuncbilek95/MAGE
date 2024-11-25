/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Data/Guid.h"
#include "Engine/Data/DayTime.h"
#include "Engine/Objects/IObject.h"
#include "Engine/Memory/OwnedBuffer.h"

namespace MAGE
{
	enum class ResourceType
	{
		Unknown,
		Texture,
		Mesh,
		Shader,
		Font,
		Audio,
		Script
	};

	struct ResourceProps
	{
		Guid guid;
		string resourceName;
		string relativePath;
		usize sizeInBytes;
		ResourceType type;
		DayTime lastModified;
		OwnedBuffer data;
	};

	class Resource : public IObject
	{
	public:
		Resource(const ResourceProps& desc);
		virtual ~Resource();

		virtual void Destroy() override = 0;

	protected:
		ResourceProps m_resProps;
	};
}
