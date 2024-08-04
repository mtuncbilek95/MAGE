#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/API.h>
#include <Engine/Platform/PlatformWindow.h>

namespace MAGE
{
	class WindowAPI : public API<WindowAPI>
	{
	public:
		WindowAPI() = default;
		~WindowAPI() = default;

		WeakPtr<PlatformWindow> InitializeWindow(const WindowDesc& desc)
		{
			if(mDefaultWindow == nullptr)
				return mDefaultWindow = PlatformWindow::InitializeWindow(desc);

			return mDefaultWindow;
		}

		SharedPtr<PlatformWindow> GetDefaultWindow() { return mDefaultWindow; }

	private:
		SharedPtr<PlatformWindow> mDefaultWindow;
	};
}
