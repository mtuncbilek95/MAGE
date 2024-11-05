/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Vulkan/Core/VkObject.h"
#include "Engine/Data/Memory/RawBuffer.h"
#include <vulkan/vulkan.hpp>

namespace MAGE
{
	struct BufferProps final
	{
	};

	class VBuffer final : public VkObject
	{
	public:
		VBuffer(const BufferProps& desc, VDevice* device);
		~VBuffer() override final;

		inline vk::Buffer GetVkBuffer() const { return m_buffer; }
		inline vk::DeviceAddress GetVkAddress() const { return m_address; }

		void Map() const;
		void Update(RawBuffer buffer);
		void Unmap() const;

		void Destroy() override final;

	private:
		BufferProps m_props;

		vk::Buffer m_buffer;
		vk::DeviceAddress m_address;
	};
}