#include "JobSystem.h"

namespace MAGE
{
	JobSystem::JobSystem(u32 threadCount) : m_pendingCount(0), m_shutdownFlag(false)
	{
		for (usize i = 0; i < threadCount; i++)
			m_workers.push_back(new WorkerThread(this));
	}

	JobSystem::~JobSystem()
	{
		WaitForIdle();

		m_shutdownFlag.store(true, std::memory_order_release);
		for (auto worker : m_workers) {
			delete worker;
		}
	}

	void JobSystem::SubmitJob(const function<void()>& fnc)
	{
		m_lock.Lock();
		m_jobQueue.push(fnc);
		m_pendingCount.fetch_add(1, std::memory_order_release);
		m_lock.Unlock();
	}

	void JobSystem::WaitForIdle() const
	{
		while (m_pendingCount.load(std::memory_order_acquire) > 0)
		{
		}
	}
}
