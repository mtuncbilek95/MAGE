#include "DXRenderer.h"

#include <spdlog/spdlog.h>

namespace Mage
{
	void DXRenderer::Start()
	{
		WindowProps winProp = {};
		winProp.isWindowed = true;
		winProp.windowSize = { 1920, 1080 };
		winProp.windowTitle = "DX11";
		m_window.Initialize(winProp);

		if (!m_instance.Initialize())
			spdlog::error("Instance error!");

		if (!m_device.Initialize(&m_instance))
			spdlog::error("Device error!");

		SwapchainProps swapProp = {};
		swapProp.window = &m_window;
		DXSwapchain swapchain;
		m_swapchain.Initialize(swapProp, &m_device);

		if (!m_rtv.Initialize(&m_swapchain))
			spdlog::error("RTV error!");
	}
}