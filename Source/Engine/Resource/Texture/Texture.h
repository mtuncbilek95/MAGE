/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "../Core/Resource.h"

#include "Engine/Vulkan/Buffer/VBuffer.h"
#include "Engine/Vulkan/Image/VImage.h"
#include "Engine/Vulkan/Image/VImageView.h"

namespace MAGE
{
	struct TextureProps final
	{
		Math::Vec3u texSize = {};
		u32 depthBit = 8;
		u32 channelCount = 4;
	};

	class Texture : public Resource
	{
	public:
		Texture(const TextureProps& texDesc, const ResourceProps& resDesc);
		virtual ~Texture() override;

		virtual void GenerateTexture();

		virtual bool Load() override;
		virtual bool Unload() override;

		virtual void Serialize(const path& absPath) override;
		virtual void Deserialize(const path& relPath) override;

		void Destroy() override final;

	private:
		TextureProps m_texProps;

		Owned<VBuffer> m_stage;
		Owned<VImage> m_image;
		Owned<VImageView> m_view;
	};
}
