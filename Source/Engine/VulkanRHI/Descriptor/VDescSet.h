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
	class VDescLayout;
	class VDescPool;
	class VDstBuffer;
	class VImageView;
	class VSampler;

	struct DescSetProps final
	{
		VDescLayout* layout;
		VDescPool* pool;
	};

	union BufferEntry final
	{
		VDstBuffer* uniform;
		VImageView* image;
	};

	struct UpdateEntry final
	{
		BufferEntry entry;
		VSampler* sampler;
		VkDescriptorType type;
		u32 count;
		u32 arrayElement;
		u64 bufferOffset;
		u32 binding;
	};

	struct UpdateProps final
	{
		Vector<UpdateEntry> entries;
	};

	class VDescSet final : public VObject
	{
	public:
		VDescSet(const DescSetProps& desc, VDevice* device);
		~VDescSet() override final;

		inline VkDescriptorSet GetSet() const { return m_set; }

		void UpdateSet(const UpdateProps& desc) const;
		void Destroy() override final;

	private:
		VkDescriptorSet m_set;

		DescSetProps m_props;
	};
}
