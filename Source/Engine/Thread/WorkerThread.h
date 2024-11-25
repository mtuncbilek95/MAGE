/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"

#include <thread>
#include <atomic>

namespace MAGE
{
	class JobSystem;

	class WorkerThread final
	{
	public:
		WorkerThread(JobSystem* js);
		~WorkerThread();

		void ThreadRun();

	private:
		JobSystem* m_system;
		std::thread m_thread;
		std::atomic<bool> m_terminate;
	};
}
