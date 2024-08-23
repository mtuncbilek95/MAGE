#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Job/JobSystem.h>

namespace MAGE
{
	class RendererJobSystem : public JobSystem
	{
	public:
		RendererJobSystem(u32 threadCount = 4, WorkerThreadPriority priority = WorkerThreadPriority::Normal);
		~RendererJobSystem() override = default;
	};
}