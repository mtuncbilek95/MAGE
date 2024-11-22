#include "IndWindow.h"

#include "Engine/ErrorHandler/ErrorChecker.h"

namespace MAGE
{
	IndWindow::IndWindow(const WindowProps& desc) : m_props(desc), m_allMonitors(MonitorHelper::GetAllMonitors())
	{
		ErrorUtils::LogAssert(glfwInit(), "GLFW Error", "Failed to initialize GLFW!");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

		m_handle = glfwCreateWindow(m_props.windowSize.x, m_props.windowSize.y, m_props.windowTitle.c_str(), nullptr, nullptr);
		ErrorUtils::LogAssert(m_handle, "GLFW Error", "Failed to create GLFW window!");

		glfwSetWindowUserPointer(m_handle, this);

		u32 index = 0;
		for (auto& monitor : m_allMonitors)
		{
			if (monitor.GetIsPrimary())
			{
				m_props.windowPos = { ((monitor.GetCurrentSize().x - m_props.windowSize.x) / 2), (monitor.GetCurrentSize().y - m_props.windowSize.y) / 2};
				glfwSetWindowPos(m_handle, m_props.windowPos.x, m_props.windowPos.y);
			}
			index++;
		}
	}

	IndWindow::~IndWindow()
	{
		Destroy();
		glfwTerminate();
	}

	void IndWindow::WindowResizeCallback(const SizeCallback& callback)
	{
		m_sizeFunc = callback;

		glfwSetWindowSizeCallback(m_handle, [](GLFWwindow* window, i32 width, i32 height)
			{
				IndWindow* indWindow = static_cast<IndWindow*>(glfwGetWindowUserPointer(window));
				indWindow->m_props.windowSize = { width, height };
				indWindow->m_sizeFunc({ width, height });
			});
	}

	void IndWindow::WindowPosCallback(const PosCallback& callback)
	{
		m_posFunc = callback;
		glfwSetWindowPosCallback(m_handle, [](GLFWwindow* window, i32 x, i32 y)
			{
				IndWindow* indWindow = static_cast<IndWindow*>(glfwGetWindowUserPointer(window));
				indWindow->m_props.windowPos = { x, y };
				indWindow->m_posFunc({ x, y });
			});
	}

	void IndWindow::PollEvents()
	{
		glfwPollEvents();
	}

	void IndWindow::Show() const
	{
		glfwShowWindow(m_handle);
	}

	void IndWindow::Hide() const
	{
		glfwHideWindow(m_handle);
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
