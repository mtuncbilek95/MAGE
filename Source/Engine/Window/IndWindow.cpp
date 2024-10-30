#include "IndWindow.h"

#include "Engine/ErrorHandler/ErrorChecker.h"

constexpr i32 monitorIndex = 0;

namespace MAGE
{
	IndWindow::IndWindow(const IndWindowDesc& desc) : m_windowRes(desc.windowRes), m_mode(desc.mode),
		m_title(desc.title), m_window(nullptr), m_windowPos(0)
	{
		// Initialize GLFW
		ErrorUtils::LogAssert(glfwInit(), "GLFW Error", "Failed to initialize GLFW!");

		// Set window hints
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

		if (m_mode == WindowMode::Borderless)
		{
			glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		}

		// Create the window
		m_window = glfwCreateWindow(m_windowRes.x, m_windowRes.y, m_title.CharString(), nullptr, nullptr);
		ErrorUtils::LogAssert(m_window, "GLFW Error", "Failed to create GLFW window!");

		// Set user pointer
		glfwSetWindowUserPointer(m_window, this);
		
		// Center the window
		// TODO: Use monitor info later with independent monitor class.
		GLFWmonitor** monitors;
		i32 monitorCount;
		monitors = glfwGetMonitors(&monitorCount);
		const GLFWvidmode* mode = glfwGetVideoMode(monitors[0]);

		// Put it in the center of the second screen.
		m_windowPos = { ((mode->width - m_windowRes.x) / 2) + mode->width * monitorIndex, (mode->height - m_windowRes.y) / 2};
		glfwSetWindowPos(m_window, m_windowPos.x, m_windowPos.y);

		glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, i32 x, i32 y)
			{
				IndWindow* indWindow = static_cast<IndWindow*>(glfwGetWindowUserPointer(window));
				indWindow->m_windowPos = { x, y };
			});
	}

	IndWindow::~IndWindow()
	{
		if (m_window)
			glfwDestroyWindow(m_window);

		glfwTerminate();
	}

	void IndWindow::WindowResizeCallback(const SizeCallback& callback)
	{
		m_sizeCallback = callback;

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, i32 width, i32 height)
			{
				IndWindow* indWindow = static_cast<IndWindow*>(glfwGetWindowUserPointer(window));
				indWindow->m_windowRes = { width, height };
				indWindow->m_sizeCallback({ width, height });
			});
	}

	void IndWindow::WindowPosCallback(const PosCallback& callback)
	{
		m_posCallback = callback;

		glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, i32 x, i32 y)
			{
				IndWindow* indWindow = static_cast<IndWindow*>(glfwGetWindowUserPointer(window));
				indWindow->m_windowPos = { x, y };
				indWindow->m_posCallback({ x, y });
			});
	}

	void IndWindow::PollEvents()
	{
		glfwPollEvents();
	}

	void IndWindow::Destroy()
	{
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);

		if (m_window)
		{
			glfwDestroyWindow(m_window);
			m_window = nullptr;
		}
	}
} // namespace Delusion
