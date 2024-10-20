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
	struct QueueProps final
	{
		u32 m_familyIndex;
		VkQueueFlags m_flags;
		VkQueue m_queue;
	};

	class VQueue final : public VObject
	{
	public:
		VQueue(const QueueProps& desc, VDevice* device);
		~VQueue() override final;

		inline VkQueue GetQueue() const { return m_props.m_queue; }
		inline u32 GetFamilyIndex() const { return m_props.m_familyIndex; }
		inline VkQueueFlags GetFlags() const { return m_props.m_flags; }

		void Destroy() override final;

	private:
		QueueProps m_props;
	};
}
