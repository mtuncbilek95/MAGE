#pragma once

#include "Engine/Core/Definitions.h"
#include "Engine/Core/StdFix.h"
#include "Engine/Object/IObject.h"
#include "Engine/Window/WindowDesc.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace MAGE
{
	class PlatformWindow : public IObject
	{
		friend class GraphicsDevice;
		friend class WindowAPI;
	public:
		static SharedPtr<PlatformWindow> InitializeWindow(const WindowDesc& desc);

	public:
		PlatformWindow(const WindowDesc& desc);
		virtual ~PlatformWindow() override;

		void Show();
		void Hide();

		void PollEvents();

		const Math::Vec2u& GetWindowResolution() const { return mWindowSize; }
		const Math::Vec2i& GetWindowPosition() const { return mWindowPosition; }
		const String& GetWindowTitle() const { return mWindowTitle; }
		const WindowMode& GetWindowMode() const { return mWindowMode; }
		const b8 IsVisible() const { return mVisible; }
		const b8 IsMinimized() const { return glfwGetWindowAttrib(mGLFWHandle, GLFW_ICONIFIED); }
		const b8 IsMaximized() const { return glfwGetWindowAttrib(mGLFWHandle, GLFW_MAXIMIZED); }
		const b8 IsClosed() const { return glfwWindowShouldClose(mGLFWHandle); }

#if defined(MAGE_WINDOWS)
		HWND GetWindowHandle() const { return glfwGetWin32Window(mGLFWHandle); }
		HINSTANCE GetInstanceHandle() const { return GetModuleHandle(NULL); }
#endif

#if defined(MAGE_LINUX)
		Display* GetDisplayHandle() const { return glfwGetX11Display(); }
		Window GetWindowHandle() const { return glfwGetX11Window(mGLFWHandle); }
#endif

		GLFWwindow* GetGLFWHandle() const { return mGLFWHandle; }

		void SetWindowResolution(const Math::Vec2u& resolution);
		void SetWindowPosition(const Math::Vec2i& position);
		void SetWindowTitle(const String& title);
		void SetWindowMode(const WindowMode& mode);
		void SetVisible(b8 visible);

	private:
		Math::Vec2u mWindowSize;
		Math::Vec2i mWindowPosition;
		String mWindowTitle;
		WindowMode mWindowMode;
		b8 mVisible;

		GLFWwindow* mGLFWHandle;
	};
}
