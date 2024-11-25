/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Thread/Spinlock.h"
#include "Engine/Thread/WorkerThread.h"

namespace MAGE
{
	class JobSystem final
	{
		friend class WorkerThread;

	public:
		JobSystem(u32 threadCount = 4);
		~JobSystem();

		void SubmitJob(const function<void()>& fnc);
		void WaitForIdle() const;
		void Shutdown();

	private:
		queue<function<void()>> m_jobQueue;
		vector<WorkerThread*> m_workers;

		std::atomic<int> m_pendingCount;
		std::atomic<bool> m_shutdownFlag;
		std::condition_variable m_waiter;
		std::mutex m_waitLock;

		Spinlock m_lock;
	};
}
