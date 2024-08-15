#include "PlatformMonitor.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#if defined(MAGE_WINDOWS)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

namespace MAGE
{
	Vector<Monitor> PlatformMonitor::GetAllMonitors()
	{
		i32 monitorCount;
		GLFWmonitor** monitorsAsGLFW = glfwGetMonitors(&monitorCount);

		Vector<Monitor> monitors(monitorCount);

		for (i32 i = 0; i < monitorCount; i++)
		{
			const GLFWvidmode* videoMode = glfwGetVideoMode(monitorsAsGLFW[i]);

			i32 monitorX, monitorY;
			glfwGetMonitorPos(monitorsAsGLFW[i], &monitorX, &monitorY);

			const char* name = glfwGetMonitorName(monitorsAsGLFW[i]);

			MonitorDesc desc = {};
			desc.MonitorName = name;
			desc.MonitorResolution = Vec2u((u32)videoMode->width, (u32)videoMode->height);
			desc.MonitorPosition = Vec2i(monitorX, monitorY);
			desc.IsPrimary = i == 0;

			monitors.push_back(Monitor(desc));
		}

		return monitors;
	}

	Monitor PlatformMonitor::GetPrimaryMonitor()
	{
		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();

		const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);

		i32 monitorX, monitorY;
		glfwGetMonitorPos(primaryMonitor, &monitorX, &monitorY);

		const char* name = glfwGetMonitorName(primaryMonitor);

		MonitorDesc desc = {};
		desc.MonitorName = name;
		desc.MonitorResolution = Vec2u((u32)videoMode->width, (u32)videoMode->height);
		desc.MonitorPosition = Vec2i(monitorX, monitorY);
		desc.IsPrimary = true;

		return Monitor(desc);
	}
}
