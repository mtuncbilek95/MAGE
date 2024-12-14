#include "DXSwapchain.h"

#include "Engine/D3D/Device/DXDevice.h"
#include "Engine/D3D/Core/DXAssert.h"

#include "Engine/Window/IndWindow.h"

namespace Mage
{
	b8 DXSwapchain::Initialize(const SwapchainProps& desc, DXDevice* device)
	{
		m_rootDevice = device;

		DXGI_SWAP_CHAIN_DESC swapInfo = {};
		swapInfo.BufferDesc.Width = desc.imgRes.x;
		swapInfo.BufferDesc.Height = desc.imgRes.y;
		swapInfo.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapInfo.BufferDesc.RefreshRate.Numerator = 60;
		swapInfo.BufferDesc.RefreshRate.Denominator = 1;
		swapInfo.SampleDesc.Count = 1;
		swapInfo.SampleDesc.Quality = 0;
		swapInfo.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapInfo.BufferCount = 2;
		swapInfo.OutputWindow = desc.window->GetNativeHandle();
		swapInfo.Windowed = desc.window->GetIsWindowed();
		swapInfo.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		if (!Assertion::DXAssert(device->GetDXInstance()->CreateSwapChain(device->GetDXDevice(), &swapInfo, m_swapchain.GetAddressOf()), "DXSwapchain"))
			return false;

		return true;
	}

	void DXSwapchain::Present(b8 synced)
	{
		m_swapchain->Present(static_cast<int>(synced), 0);
	}

	void DXSwapchain::Destroy()
	{
	}
}