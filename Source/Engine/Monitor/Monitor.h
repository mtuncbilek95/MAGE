#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Monitor/MonitorDesc.h"

namespace MAGE
{
	class Monitor
	{
	public:
		Monitor() = default;
		Monitor(const MonitorDesc& desc) : mMonitorName(desc.MonitorName), 
			mMonitorResolution(desc.MonitorResolution), mMonitorPosition(desc.MonitorPosition),
			mIsPrimary(desc.IsPrimary) 
		{}
		~Monitor() = default;

		const String& GetMonitorName() const { return mMonitorName; }
		const Vec2u& GetMonitorResolution() const { return mMonitorResolution; }
		const Vec2i& GetMonitorPosition() const { return mMonitorPosition; }
		const b8 IsPrimary() const { return mIsPrimary; }

	private:
		String mMonitorName;
		Vec2u mMonitorResolution;
		Vec2i mMonitorPosition;
		b8 mIsPrimary;
	};
}
