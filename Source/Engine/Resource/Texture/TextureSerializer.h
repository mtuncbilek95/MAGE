/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Objects/Singleton.h"
#include "Engine/Resource/Core/Resource.h"
#include "Engine/Resource/Texture/Texture.h"

namespace MAGE
{
	class TextureSerializer final : public Singleton<TextureSerializer>
	{
	public:
		template<typename T, typename = std::enable_if<std::is_base_of_v<Texture, T>>>
		Shared<T> SerializeTexture(const ResourceProps& desc)
		{
			// Read texture file such as .png, .tga, .tiff then make it mage object
			Shared<T> tex = MakeShared<T>(desc);

			return Shared<T>();
		}

		//Shared<Texture> DeserializeTexture(const path& absPath);
	};
}
