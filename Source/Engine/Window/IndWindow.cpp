#include "IndWindow.h"

#include <iostream>
#include <spdlog/spdlog.h>

namespace Mage
{
	IndWindow::~IndWindow()
	{
		Destroy();
		glfwTerminate();
	}

	void IndWindow::Initialize(const WindowProps& desc)
	{
		m_props = desc;

		if (!glfwInit())
		{
			spdlog::error("Failed to generate glfw!");
			exit(-1);
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

		m_handle = glfwCreateWindow(m_props.windowSize.x, m_props.windowSize.y, m_props.windowTitle.c_str(), nullptr, nullptr);

		if (!m_handle)
		{
			spdlog::error("Failed to create independent glfw window!");
			glfwTerminate();
			exit(-1);
		}

		glfwSetWindowUserPointer(m_handle, this);

		// TODO: Hardcoded position will be changed.
		glfwSetWindowPos(m_handle, (2560 - 1920) / 2, (1440 - 1080) / 2);
	}

	void IndWindow::Destroy()
	{
		if (m_handle)
		{
			glfwSetWindowShouldClose(m_handle, GLFW_TRUE);
			glfwDestroyWindow(m_handle);
		}
		m_handle = nullptr;
	}
}