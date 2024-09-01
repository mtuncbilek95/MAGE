#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Job/WorkerThreadPriority.h"
#include "Engine/Thread/ThreadJob.h"
#include "Engine/Platform/PlatformThread.h"
#include "Engine/Platform/PlatformCriticalSection.h"

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
		WorkerThread(WorkerThreadPriority priority = WorkerThreadPriority::WTP_Normal);
		~WorkerThread();

		void AddJob(VoidFunction job);
		void AddJob(SharedPtr<ThreadJob> job);
		void RequestQuit();

	private:
		void ThreadLoop();

	private:
		SharedPtr<PlatformThread> mThread;
		SharedPtr<PlatformCriticalSection> mCriticalSection;
		ThreadJob mLoopJob;
		Queue<SharedPtr<ThreadJob>> mJobQueue;

		bool mQuitRequested = false;
	};
}
