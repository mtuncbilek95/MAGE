#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Platform/PlatformThread.h>
#include <Engine/Platform/PlatformCriticalSection.h>

namespace MAGE
{
	/**
	 * @class WorkerThread
	 * 
	 * @brief WorkerThread is responsible for executing jobs in infinite loop. This means that the WorkerThread will be running, 
	 * waiting for jobs to be added to the queue.
	 */
	class WorkerThread
	{
	public:
		WorkerThread();
		~WorkerThread();

		void ThreadLoop();
		void AddJob(ThreadJob* job);

	private:
		SharedPtr<PlatformThread> mThread;
		SharedPtr<PlatformCriticalSection> mCriticalSection;
		ThreadJob* mCurrentJob;
		// TODO: Add a queue in the WorkerThread class later
	};
}
