/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Definitions/Definitions.h"
#include "Engine/Definitions/StdNames.h"
#include "Engine/Definitions/GlmNames.h"
#include "Engine/Containers/IObject.h"

#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>

namespace Mage
{
	struct WindowProps final
	{
		string windowTitle = "DefaultWindow";
		Math::Vec2u windowSize = { 1920, 1080 };
		Math::Vec2i windowPos = { 0, 0 };
		b8 isWindowed = true;
	};

	class IndWindow final : public IObject
	{
	public:
		IndWindow() = default;
		~IndWindow() override final;

		void Initialize(const WindowProps& desc);
		void PollEvents() { glfwPollEvents(); }

		void Show() const { glfwShowWindow(m_handle); }
		void Hide() const { glfwHideWindow(m_handle); }

		void Destroy() override final;

		const Math::Vec2u& GetWindowRes() const { return m_props.windowSize; }
		const Math::Vec2i& GetWindowPos() const { return m_props.windowPos; }

		string GetWindowName() const { return m_props.windowTitle; }
		b8 GetIsWindowed() const { return m_props.isWindowed; }
		b8 IsClosed() const { return glfwWindowShouldClose(m_handle); }

#if defined(MAGE_WINDOWS)
		HWND GetNativeHandle() const { return glfwGetWin32Window(m_handle); }
		HINSTANCE GetNativeInstance() const { return GetModuleHandle(nullptr); }
#endif

		GLFWwindow* GetGLFWWindow() const { return m_handle; }

	private:
		WindowProps m_props;
		GLFWwindow* m_handle;
	};
}
