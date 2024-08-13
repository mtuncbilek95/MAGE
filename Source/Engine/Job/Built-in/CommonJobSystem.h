#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/API.h> 
#include <Engine/Job/IJobSystem.h>

namespace MAGE
{
	class CommonJobSystem final : public API<CommonJobSystem>, public IJobSystem
	{
	public:
		CommonJobSystem();
		~CommonJobSystem();

		virtual bool Initialize(u16 threadCount) override;
		virtual void AddJob(ThreadJob* job) override;
	};
}
