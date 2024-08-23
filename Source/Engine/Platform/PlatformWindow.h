#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>
#include <Engine/Object/IObject.h>
#include <Engine/Window/WindowDesc.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace MAGE
{
	class PlatformWindow final : public IObject
	{
		friend class GraphicsDevice;
		friend class WindowAPI;
	public:
		static SharedPtr<PlatformWindow> InitializeWindow(const WindowDesc& desc);

	public:
		explicit PlatformWindow(const WindowDesc& desc);
		~PlatformWindow() override;

		void Show();
		void Hide();

		void PollEvents();

		const Vec2u& GetWindowResolution() const { return mWindowSize; }
		const Vec2i& GetWindowPosition() const { return mWindowPosition; }
		const String& GetWindowTitle() const { return mWindowTitle; }
		const WindowMode& GetWindowMode() const { return mWindowMode; }
		b8 IsVisible() const { return mVisible; }
		b8 IsMinimized() const { return glfwGetWindowAttrib(mGLFWHandle, GLFW_ICONIFIED); }
		b8 IsMaximized() const { return glfwGetWindowAttrib(mGLFWHandle, GLFW_MAXIMIZED); }
		b8 IsClosed() const { return glfwWindowShouldClose(mGLFWHandle); }

#if defined(MAGE_WINDOWS)
		HWND GetWindowHandle() const { return glfwGetWin32Window(mGLFWHandle); }
		HINSTANCE GetInstanceHandle() const { return GetModuleHandle(NULL); }
#endif

#if defined(MAGE_LINUX)
		Display* GetDisplayHandle() const { return glfwGetX11Display(); }
		Window GetWindowHandle() const { return glfwGetX11Window(mGLFWHandle); }
#endif

		GLFWwindow* GetGLFWHandle() const { return mGLFWHandle; }

		void SetWindowResolution(const Vec2u& resolution);
		void SetWindowPosition(const Vec2i& position);
		void SetWindowTitle(const String& title);
		void SetWindowMode(const WindowMode& mode);
		void SetVisible(b8 visible);

	private:
		Vec2u mWindowSize;
		Vec2i mWindowPosition;
		String mWindowTitle;
		WindowMode mWindowMode;
		b8 mVisible;

		GLFWwindow* mGLFWHandle;
	};
}
