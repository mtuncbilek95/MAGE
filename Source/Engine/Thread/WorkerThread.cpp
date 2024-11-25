#include "WorkerThread.h"
#include "JobSystem.h"

namespace MAGE
{
	WorkerThread::WorkerThread(JobSystem* js) : m_system(js), m_terminate(false), m_thread(&WorkerThread::ThreadRun, this)
	{
	}

	WorkerThread::~WorkerThread()
	{
		m_terminate.store(true, std::memory_order_release);
		if (m_thread.joinable())
			m_thread.join();
	}

	void WorkerThread::ThreadRun()
	{
		while (!m_terminate.load(std::memory_order_acquire) && !m_system->m_shutdownFlag.load(std::memory_order_acquire))
		{
			m_system->m_lock.Lock();
			if (!m_system->m_jobQueue.empty())
			{
				auto job = m_system->m_jobQueue.front();
				m_system->m_jobQueue.pop();
				m_system->m_lock.Unlock();

				job();

				m_system->m_pendingCount.fetch_sub(1, std::memory_order_release);
			}
			else
				m_system->m_lock.Unlock();
		}
	}
}
