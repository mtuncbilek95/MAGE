/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "../Abstraction/VObject.h"

namespace MAGE
{
	class VImageView;
	class VRenderPass;

	struct FramebufferProps
	{
		VImageView* imageView;
		VRenderPass* renderPass;
	};

	class VFramebuffer final : public VObject
	{
	public:
		VFramebuffer(const FramebufferProps& props, VDevice* device);
		~VFramebuffer() override final;

		inline VkFramebuffer GetBuffer() const { return m_framebuffer; }
		Math::Vec2u GetRenderArea() const;

		void Destroy() override final;

	private:
		FramebufferProps m_props; 
		VkFramebuffer m_framebuffer;
	};
}
