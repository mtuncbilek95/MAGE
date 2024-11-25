#include "Spinlock.h"

namespace MAGE
{
	void Spinlock::Lock()
	{
		while (m_flag.test_and_set(std::memory_order_acquire)) {}
	}

	void Spinlock::Unlock()
	{
		m_flag.clear(std::memory_order_release);
	}
}
