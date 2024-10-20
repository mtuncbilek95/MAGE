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
		void WindowPosCallback(const PosCallback& callback);

		void PollEvents();

		const Math::Vec2u& GetWindowRes() const { return m_windowRes; }
		const Math::Vec2i& GetWindowPos() const { return m_windowPos; }
		
		String GetTitle() const { return m_title; }
		WindowMode GetMode() const { return m_mode; }
		bool IsClosed() const { return glfwWindowShouldClose(m_window); }

		void Show() { glfwShowWindow(m_window); }
		void Hide() { glfwHideWindow(m_window); }
		void Quit() { glfwSetWindowShouldClose(m_window, true); }
		void Destroy();

#if defined(DELUSION_WINDOWS)
		HWND GetNativeHandle() const { return glfwGetWin32Window(m_window); }
		HINSTANCE GetNativeInstance() const { return GetModuleHandle(nullptr); }
#endif // DELUSION_WINDOWS
		GLFWwindow* GetGLFWWindow() const { return m_window; }

	private:
		GLFWwindow* m_window;

		Math::Vec2u m_windowRes;
		Math::Vec2i m_windowPos;

		WindowMode m_mode;
		String m_title;

		SizeCallback m_sizeCallback;
		PosCallback m_posCallback;
	};
}
