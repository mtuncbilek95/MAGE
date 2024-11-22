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
#include "Engine/Serialization/ISerialObject.h"

namespace MAGE
{
	enum class LoadState
	{
		Unloaded,
		Loading,
		Loaded
	};

	struct ResourceProps final
	{
		string absolutePath;
		string relativePath;
		Guid id;
		string name = "DefaultObject";
		LoadState state;
		usize dataSize;
		DayTime lastModified;
	};

	class IResource : public IObject, ISerialObject
	{
		friend class TextureHelper;

	public:
		virtual ~IResource() override = default;

		stringView GetAbsolutePath() const { return m_resProps.absolutePath; }
		stringView GetRelativePath() const { return m_resProps.relativePath; }
		Guid GetID() const { return m_resProps.id; }
		stringView GetName() const { return m_resProps.name; }
		usize GetSize() const { return m_resProps.dataSize; }
		DayTime GetLastModifiedTime() const { return m_resProps.lastModified; }

		virtual void Destroy() override = 0;

		virtual void Serialize() override = 0;
		virtual void Deserialize() override = 0;

	protected:
		ResourceProps m_resProps;
	};
}
