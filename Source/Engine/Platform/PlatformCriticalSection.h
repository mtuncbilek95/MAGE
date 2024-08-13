#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	class PlatformCriticalSection
	{
	public:
		static SharedPtr<PlatformCriticalSection> Create();

	public:
		PlatformCriticalSection() = default;
		virtual ~PlatformCriticalSection() = default;

		virtual b8 TryEnter() = 0;
		virtual void Enter() = 0;
		virtual void Exit() = 0;
	};
}
