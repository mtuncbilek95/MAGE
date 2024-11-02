/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Rendering/RenderContext.h"

#include "Engine/VulkanRHI/Buffer/VStageBuffer.h"
#include "Engine/VulkanRHI/Image/VImage.h"
#include "Engine/VulkanRHI/Image/VImageView.h"

namespace MAGE
{
	struct TextureProps final
	{
		Math::Vec2u imageSize;
		u32 channelCount;
	};

	class Texture
	{
	public:
		Texture();
		virtual ~Texture();

		virtual void CreateImage(const String& imagePath);
		virtual void CreateView() = 0;
		void TransferData();
		
		void Destroy();

	protected:
		Owned<VStageBuffer> m_buffer;
		Owned<VImage> m_image;
		Owned<VImageView> m_view;

		Owned<VCmdBuffer> m_transferBuffer;

		TextureProps m_props;
		Gfx::Context& m_context;
	};
}
