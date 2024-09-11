#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Application/ApplicationModule.h"

namespace MAGE
{
	class ImGuiWindowAPI;
	class EDITOR_API ImGuiWindowModule : public ApplicationModule
	{
	public:
		ImGuiWindowModule();
		virtual ~ImGuiWindowModule() override = default;

		virtual void Start() noexcept override;
		virtual void Update() noexcept override;
		virtual void Stop() noexcept override;

	protected:
		virtual b8 OnInitialize() noexcept override;
		virtual void OnPreUpdate() noexcept override;
		virtual void OnPostUpdate() noexcept override;

	private:
		ImGuiWindowAPI* mImGuiAPI;
	};
}
