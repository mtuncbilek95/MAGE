#include "Application.h"

#if defined(MAGE_WINDOWS)
#include <Windows.h>
#endif

namespace MAGE
{
	void Application::Run()
	{
		OnInitialize();

		for (auto& pModule : mTotalModules)
		{
			if (pModule->GetState() == ApplicationModuleState::NeedValidation)
			{
				if (pModule->OnInitialize())
				{
					CORE_LOG(M_VERBOSE, "Module %s validated", pModule->GetModuleName().c_str());
					pModule->SetState(ApplicationModuleState::Validated);
				}
				else
				{
					CORE_LOG(M_WARNING, "Module %s invalidated", pModule->GetModuleName().c_str());
					pModule->SetState(ApplicationModuleState::Invalidated);
				}
			}
		}

		for (auto& module : mTotalModules)
		{
			if (module->GetState() == ApplicationModuleState::Validated)
			{
				module->Start();
			}
		}

		while (!mQuitRequested)
		{
			for (auto& module : mTotalModules)
			{
				if (module->GetState() == ApplicationModuleState::Validated)
				{
					module->OnPreUpdate();
				}
			}

			for (auto& module : mTotalModules)
			{
				if (module->GetState() == ApplicationModuleState::Validated)
				{
					module->Update();
				}
			}

			for (auto& module : mTotalModules)
			{
				if (module->GetState() == ApplicationModuleState::Validated)
				{
					module->OnPostUpdate();
				}
			}
		}

		// Stop all modules reversed
		for (auto it = mTotalModules.rbegin(); it != mTotalModules.rend(); ++it)
		{
			if ((*it)->GetState() == ApplicationModuleState::Validated)
			{
				(*it)->Stop();
			}
		}
	}

	void Application::QuitReason(const String& reason)
	{
		CORE_LOG(M_WARNING, "%s", reason.c_str());
	}
}
