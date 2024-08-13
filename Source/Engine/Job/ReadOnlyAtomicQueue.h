#pragma once

#include <Engine/Core/Core.h>
#include <atomic>

namespace MAGE
{
	// TODO: Implement ReadOnlyAtomicQueue
	class ReadOnlyAtomicQueue
	{
	public:
		ReadOnlyAtomicQueue() = default;
		~ReadOnlyAtomicQueue() = default;

	private:
		std::atomic<u64> mSize;

	};
}
