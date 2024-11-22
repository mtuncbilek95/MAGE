/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	struct MonitorProps final
	{
		string monitorName;
		Math::Vec2u currentSize;
		vector<Math::Vec2u> supportedSizes;
		u32 currentHertz;
		b8 isPrimary;
	};

	class Monitor final
	{
	public:
		Monitor() = default;
		Monitor(const MonitorProps& desc);
		~Monitor();

		const string& GetName() const { return m_props.monitorName; }
		const Math::Vec2u& GetCurrentSize() const { return m_props.currentSize; }
		const vector<Math::Vec2u>& GetAllSizes() const { return m_props.supportedSizes; }
		u32 GetCurrentHertz() const { return m_props.currentHertz; }
		b8 GetIsPrimary() const { return m_props.isPrimary; }

	private:
		MonitorProps m_props;
	};

	namespace MonitorHelper
	{
		Monitor GetPrimaryMonitor();
		vector<Monitor> GetAllMonitors();
	}
}
