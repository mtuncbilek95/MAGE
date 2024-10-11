#include "IndWindow.h"

#include "Engine/ErrorHandler/ErrorChecker.h"

constexpr i32 monitorIndex = 0;

namespace MAGE
{
	IndWindow::IndWindow(const IndWindowDesc& desc) : m_WindowRes(desc.WindowRes), m_Mode(desc.Mode),
		m_Title(desc.Title), m_Window(nullptr), m_WindowPos(0)
	{
		// Initialize GLFW
		ErrorUtils::LogAssert(glfwInit(), "GLFW Error", "Failed to initialize GLFW!");

		// Set window hints
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

		// Create the window
		m_Window = glfwCreateWindow(m_WindowRes.x, m_WindowRes.y, m_Title.c_str(), nullptr, nullptr);
		ErrorUtils::LogAssert(m_Window, "GLFW Error", "Failed to create GLFW window!");

		// Set user pointer
		glfwSetWindowUserPointer(m_Window, this);
		
		// Center the window
		// TODO: Use monitor info later with independent monitor class.
		GLFWmonitor** monitors;
		i32 monitorCount;
		monitors = glfwGetMonitors(&monitorCount);
		const GLFWvidmode* mode = glfwGetVideoMode(monitors[0]);

		// Put it in the center of the second screen.
		m_WindowPos = { ((mode->width - m_WindowRes.x) / 2) + mode->width * monitorIndex, (mode->height - m_WindowRes.y) / 2};
		glfwSetWindowPos(m_Window, m_WindowPos.x, m_WindowPos.y);

		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, i32 x, i32 y)
			{
				IndWindow* indWindow = static_cast<IndWindow*>(glfwGetWindowUserPointer(window));
				indWindow->m_WindowPos = { x, y };
			});
	}

	IndWindow::~IndWindow()
	{
		if (m_Window)
			glfwDestroyWindow(m_Window);

		glfwTerminate();
	}

	void IndWindow::WindowResizeCallback(SizeCallback callback)
	{
		m_SizeCallback = callback;

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, i32 width, i32 height)
			{
				IndWindow* indWindow = static_cast<IndWindow*>(glfwGetWindowUserPointer(window));
				indWindow->m_WindowRes = { width, height };
				indWindow->m_SizeCallback({ width, height });
			});
	}

	void IndWindow::PollEvents()
	{
		glfwPollEvents();
	}

	void IndWindow::Destroy()
	{
		glfwSetWindowShouldClose(m_Window, GLFW_TRUE);

		if (m_Window)
		{
			glfwDestroyWindow(m_Window);
			m_Window = nullptr;
		}
	}
} // namespace Delusion
