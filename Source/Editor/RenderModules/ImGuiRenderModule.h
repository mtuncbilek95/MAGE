#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Application/ApplicationModule.h"

namespace MAGE
{
	class ImGuiAPI;
	class EDITOR_API ImGuiRenderModule : public ApplicationModule
	{
	public:
		ImGuiRenderModule();
		virtual ~ImGuiRenderModule() override = default;

		virtual void Start() noexcept override;
		virtual void Update() noexcept override;
		virtual void Stop() noexcept override;

	protected:
		virtual b8 OnInitialize() noexcept override;
		virtual void OnPreUpdate() noexcept override;
		virtual void OnPostUpdate() noexcept override;

	private:
		ImGuiAPI* mImGuiAPI;
	};
}
