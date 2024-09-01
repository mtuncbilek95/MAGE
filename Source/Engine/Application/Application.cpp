#include "Application.h"

#include <spdlog/spdlog.h>

namespace MAGE
{
	void Application::Run()
	{
		spdlog::set_level(spdlog::level::debug);

		OnInitialize();

		for (auto& pModule : mTotalModules)
		{
			if (pModule->GetState() == ApplicationModuleState::AMS_NeedValidation)
			{
				if (pModule->OnInitialize())
				{
					spdlog::info("Module {} validated", pModule->GetModuleName());
					pModule->SetState(ApplicationModuleState::AMS_Validated);
				}
				else
				{
					spdlog::warn("Module {} invalidated", pModule->GetModuleName());
					pModule->SetState(ApplicationModuleState::AMS_Invalidated);
				}
			}
		}

		for (auto& module : mTotalModules)
		{
			if (module->GetState() == ApplicationModuleState::AMS_Validated)
			{
				module->Start();
			}
		}

		while (!mQuitRequested)
		{
			for (auto& module : mTotalModules)
			{
				if (module->GetState() == ApplicationModuleState::AMS_Validated)
				{
					module->OnPreUpdate();
				}
			}

			for (auto& module : mTotalModules)
			{
				if (module->GetState() == ApplicationModuleState::AMS_Validated)
				{
					module->Update();
				}
			}

			for (auto& module : mTotalModules)
			{
				if (module->GetState() == ApplicationModuleState::AMS_Validated)
				{
					module->OnPostUpdate();
				}
			}
		}

		// Stop all modules reversed
		for (auto it = mTotalModules.rbegin(); it != mTotalModules.rend(); ++it)
		{
			if ((*it)->GetState() == ApplicationModuleState::AMS_Validated)
			{
				(*it)->Stop();
			}
		}
	}

	void Application::QuitReason(const String& reason)
	{
		spdlog::warn("{}", reason);
	}
}
