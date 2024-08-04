#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/IObject.h >
#include <Engine/Application/ApplicationModuleState.h>

namespace MAGE
{
	class ApplicationModule : public IObject
	{
		friend class Application;

	public:
		ApplicationModule() = default;
		virtual ~ApplicationModule() override = default;

		FORCEINLINE Application* GetOwnerApp() { return mOwnerApp; }
		FORCEINLINE const String& GetModuleName() const { return mModuleName; }
		FORCEINLINE ApplicationModuleState GetState() const { return mState; }

		virtual void Start() noexcept = 0;
		virtual void Update() noexcept = 0;
		virtual void Stop() noexcept = 0;

	protected:
		virtual bool OnInitialize() noexcept = 0;
		virtual void OnPreUpdate() noexcept = 0;
		virtual void OnPostUpdate() noexcept = 0;

		void SetOwnerApplication(Application* app) { mOwnerApp = app; }
		void SetState(ApplicationModuleState state) { mState = state; }
		void SetModuleName(const String& name) { mModuleName = name; }

	private:
		Application* mOwnerApp;
		ApplicationModuleState mState = ApplicationModuleState::NeedValidation;
		String mModuleName;
	};
}
