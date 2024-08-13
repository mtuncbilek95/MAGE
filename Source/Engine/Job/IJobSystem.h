#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/API.h>

namespace MAGE
{
	class WorkerThread;
	class ThreadJob;

	class IJobSystem
	{
	public:
		IJobSystem() = default;
		virtual ~IJobSystem() = default;

		virtual bool Initialize(u16 threadCount) = 0;
		virtual void AddJob(ThreadJob* job) = 0;

	protected:
		DArray<SharedPtr<WorkerThread>> mWorkerThreads;
		s64 mDistributionIndex = 0;
	};
}
