#include "WorkerThread.h"

namespace MAGE
{
	WorkerThread::WorkerThread(WorkerThreadPriority priority) : mLoopJob(ThreadJob([this] { this->ThreadLoop(); }))
	{
		mThread = PlatformThread::CreatePlatformThread(&mLoopJob);
		mThread->SetPriority(static_cast<int>(priority));
	}

	WorkerThread::~WorkerThread()
	{
		mCriticalSection->Enter();
		mQuitRequested = true;
		mCriticalSection->Exit();

		mCriticalSection->Enter();
		while (!mJobQueue.empty())
			mJobQueue.pop();
		mCriticalSection->Exit();
	}

	void WorkerThread::AddJob(VoidFunction job)
	{
		mCriticalSection->Enter();
		mJobQueue.push(MakeShared<ThreadJob>(job));
		mCriticalSection->Exit();
	}

	void WorkerThread::AddJob(SharedPtr<ThreadJob> job)
	{
		mCriticalSection->Enter();
		mJobQueue.push(job);
		mCriticalSection->Exit();
	}

	void WorkerThread::RequestQuit()
	{
		mCriticalSection->Enter();
		mQuitRequested = true;
		mCriticalSection->Exit();
	}

	void WorkerThread::ThreadLoop()
	{
		while (!mQuitRequested)
		{
			mCriticalSection->Enter();
			if (mJobQueue.empty())
				PlatformThread::SleepThread(1);

			if (!mJobQueue.empty())
			{
				auto job = mJobQueue.front();
				job->Execute();
				mJobQueue.pop();
			}
			mCriticalSection->Exit();
		}
	}
}
