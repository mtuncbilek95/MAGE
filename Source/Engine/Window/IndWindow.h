/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Objects/IObject.h"
#include "Engine/Monitor/Monitor.h"

#include <GLFW/glfw3.h>
#include <glfw/glfw3native.h>

namespace MAGE
{
	struct WindowProps final
	{
		string windowTitle;
		Math::Vec2u windowSize;
		Math::Vec2i windowPos;
		b8 isWindowed;
	};

	class IndWindow final : public IObject
	{
		using SizeCallback = function<void(Math::Vec2u)>;
		using PosCallback = function<void(Math::Vec2i)>;

	public:
		IndWindow(const WindowProps& desc);
		~IndWindow();

		void WindowResizeCallback(const SizeCallback& callback);
		void WindowPosCallback(const PosCallback& callback);

		void PollEvents();

		const Math::Vec2u& GetWindowRes() const { return m_props.windowSize; }
		const Math::Vec2i& GetWindowPos() const { return m_props.windowPos; }

		string GetWindowName() const { return m_props.windowTitle; }
		b8 GetIsWindowed() const { return m_props.isWindowed; }
		b8 IsClosed() const { return glfwWindowShouldClose(m_handle); }

#if defined(DELUSION_WINDOWS)
		HWND GetNativeHandle() const { return glfwGetWin32Window(m_handle); }
		HINSTANCE GetNativeInstance() const { return GetModuleHandle(nullptr); }
#endif // DELUSION_WINDOWS
		GLFWwindow* GetGLFWWindow() const { return m_handle; }

		void Show() const;
		void Hide() const;
		void Destroy() override final;

	private:
		WindowProps m_props;
		GLFWwindow* m_handle;

		SizeCallback m_sizeFunc;
		PosCallback m_posFunc;

		vector<Monitor> m_allMonitors;
	};
}
