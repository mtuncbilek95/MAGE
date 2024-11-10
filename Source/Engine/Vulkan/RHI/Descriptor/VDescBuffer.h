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
#include "Engine/Vulkan/RHI/Memory/VMemory.h"
#include <vulkan/vulkan.hpp>

namespace MAGE
{
	class VDescLayout;
	class VBuffer;
	class VImage;
	class VSampler;

	struct DescBufferProps final
	{
		u32 resourceCount;
		VDescLayout* layout;
		vk::BufferUsageFlags usageFlags;
	};

	enum class SetupType
	{
		UniformBuffer,
		CombinedImage
	};

	struct SetupProps final
	{
		SetupType type;
		Vector<VBuffer*> pUniforms;
		Vector<VImage*> pImages;
		Vector<VSampler*> pSamplers;
	};

	class VDescBuffer final : VkObject
	{
	public:
		VDescBuffer(const DescBufferProps& desc, VDevice* device);
		~VDescBuffer() override final;

		inline vk::Buffer GetVkBuffer() const { return m_buffer; }
		inline vk::DeviceMemory GetVkMemory() const { return m_memory->m_memory; }
		vk::DeviceAddress GetVkAddress() const;

		inline u8* GetMappedData() const { return m_memory->m_mappedData; }
		inline u64 GetTotalSize() const { return m_layoutSize * m_props.resourceCount; }
		inline u64 GetMemoryOffset() const { return m_memoryOffset; }
		inline u64 GetBufferOffset() const { return m_bufferOffset; }

		void Update(RawBuffer buffer) const;

		void SetupData(const SetupProps& dataProp);
		void Destroy() override final;

	private:
		u64 AlignMemory(u64 value, u64 alignment) const;

	private:
		DescBufferProps m_props;
		VMemory* m_memory;

		vk::Buffer m_buffer;
		u64 m_layoutSize;
		u64 m_memoryOffset;
		u64 m_bufferOffset;
	};
}