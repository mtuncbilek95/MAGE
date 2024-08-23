#pragma once

#include <Engine/Object/API.h>
#include <Engine/Platform/PlatformWindow.h>

namespace MAGE
{
	class WindowAPI final : public API<WindowAPI>
	{
	public:
		WindowAPI() = default;
		~WindowAPI() override = default;

		WeakPtr<PlatformWindow> InitializeWindow(const WindowDesc& desc)
		{
			if(mDefaultWindow == nullptr)
				return mDefaultWindow = MakeShared<PlatformWindow>(desc);

			return mDefaultWindow;
		}

		SharedPtr<PlatformWindow> GetDefaultWindow() { return mDefaultWindow; }

	private:
		SharedPtr<PlatformWindow> mDefaultWindow;
	};
}
