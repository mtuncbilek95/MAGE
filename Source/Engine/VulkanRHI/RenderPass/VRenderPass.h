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
	struct PassAttachment final
	{
		VkFormat format;
		VkImageLayout initialOut;
		VkImageLayout finalOut;
	};

	struct RenderPassProps final
	{
		b8 hasColor;
		PassAttachment colorPass;
		b8 hasDepth;
		PassAttachment depthPass;

		Math::Vec4f clearColor;
		Math::Vec2f depthValue;
	};

	class VRenderPass final : public VObject
	{
	public:
		VRenderPass(const RenderPassProps& desc, VDevice* device);
		~VRenderPass() override final;

		inline VkRenderPass GetRenderPass() const { return m_renderPass; }
		inline Math::Vec4f GetColor() const { return m_props.clearColor; }
		inline Math::Vec2f GetDepth() const { return m_props.depthValue; }

		void Destroy() override final;

	private:
		VkRenderPass m_renderPass;

		RenderPassProps m_props;
	};
}
