/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>
#include <glfw/glfw3native.h>

namespace MAGE
{
	enum class WindowMode
	{
		Windowed,
		Borderless
	};

	/**
	 * @struct IndWindowDesc
	 * @brief Initialization descriptor for independent window.
	 */
	struct IndWindowDesc final
	{
		Math::Vec2u windowRes;
		WindowMode mode;
		String title;
	};

	/**
	 * @class IndWindow
	 * @brief GLFW based independent window for standalone purposes.
	 */
	class IndWindow final
	{
		using SizeCallback = Function<void(Math::Vec2u)>;
		using PosCallback = Function<void(Math::Vec2i)>;
	public:
		IndWindow(const IndWindowDesc& desc);
		~IndWindow();

		void WindowResizeCallback(const SizeCallback& callback);
		void FrameResizeCallback(const SizeCallback& callback);
		void WindowPosCallback(const PosCallback& callback);

		void PollEvents();

		const Math::Vec2u& GetWindowRes() const { return m_WindowRes; }
		const Math::Vec2i& GetWindowPos() const { return m_WindowPos; }
		
		String GetTitle() const { return m_Title; }
		WindowMode GetMode() const { return m_Mode; }
		bool IsClosed() const { return glfwWindowShouldClose(m_Window); }

		void Show() { glfwShowWindow(m_Window); }
		void Hide() { glfwHideWindow(m_Window); }
		void Quit() { glfwSetWindowShouldClose(m_Window, true); }
		void Destroy();

#if defined(DELUSION_WINDOWS)
		HWND GetNativeHandle() const { return glfwGetWin32Window(m_Window); }
		HINSTANCE GetNativeInstance() const { return GetModuleHandle(nullptr); }
#endif // DELUSION_WINDOWS

#if defined(DELUSION_LINUX)
		Window GetNativeHandle() const { return glfwGetX11Window(m_Window); }
		Display* GetNativeInstance() const { return glfwGetX11Display(); }
#endif // DELUSION_LINUX
		GLFWwindow* GetGLFWWindow() const { return m_Window; }

	private:
		GLFWwindow* m_Window;

		Math::Vec2u m_WindowRes;
		Math::Vec2i m_WindowPos;

		WindowMode m_Mode;
		String m_Title;

		SizeCallback m_SizeCallback;
		PosCallback m_PosCallback;
	};
}
