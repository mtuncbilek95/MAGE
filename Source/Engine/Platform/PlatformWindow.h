#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>
#include <Engine/Object/IObject.h >
#include <Engine/Window/WindowDesc.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#if defined(MAGE_WINDOWS)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

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

		const Vec2u& GetWindowResolution() const { return mWindowSize; }
		const Vec2i& GetWindowPosition() const { return mWindowPosition; }
		const String& GetWindowTitle() const { return mWindowTitle; }
		const WindowMode& GetWindowMode() const { return mWindowMode; }
		const bool IsVisible() const { return mVisible; }
		const bool IsMinimized() const { return glfwGetWindowAttrib(mGLFWHandle, GLFW_ICONIFIED); }
		const bool IsMaximized() const { return glfwGetWindowAttrib(mGLFWHandle, GLFW_MAXIMIZED); }
		const bool IsClosed() const { return glfwWindowShouldClose(mGLFWHandle); }

#if defined(MAGE_WINDOWS)
		HWND GetWindowHandle() const { return glfwGetWin32Window(mGLFWHandle); }
		HINSTANCE GetInstanceHandle() const { return GetModuleHandle(NULL); }
#endif

		GLFWwindow* GetGLFWHandle() const { return mGLFWHandle; }

		void SetWindowResolution(const Vec2u& resolution);
		void SetWindowPosition(const Vec2i& position);
		void SetWindowTitle(const String& title);
		void SetWindowMode(const WindowMode& mode);
		void SetVisible(bool visible);

	private:
		Vec2u mWindowSize;
		Vec2i mWindowPosition;
		String mWindowTitle;
		WindowMode mWindowMode;
		bool mVisible;

		GLFWwindow* mGLFWHandle;
	};
}
