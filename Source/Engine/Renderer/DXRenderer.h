/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Containers/Singletons.h"
#include "Engine/Definitions/GlmNames.h"
#include "Engine/Definitions/StdNames.h"

#include "Engine/Window/IndWindow.h"
#include "Engine/D3D/Instance/DXInstance.h"
#include "Engine/D3D/Device/DXDevice.h"
#include "Engine/D3D/Swapchain/DXSwapchain.h"
#include "Engine/D3D/RenderPass/DXRenderTarget.h"

namespace Mage
{
	class DXRenderer final : public SafeSingleton<DXRenderer>
	{
	public:
		DXRenderer() = default;
		~DXRenderer() = default;

		void Start();

		IndWindow* GetWindow() { return &m_window; }
		DXInstance* GetInstance() { return &m_instance; }
		DXDevice* GetDevice() { return &m_device; }
		DXSwapchain* GetSwapchain() { return &m_swapchain; }
		DXRenderTarget* GetRTV() { return &m_rtv; }

	private:
		IndWindow m_window;
		DXInstance m_instance;
		DXDevice m_device;
		DXSwapchain m_swapchain;
		DXRenderTarget m_rtv;
	};

	namespace Context
	{
		inline static IndWindow* GetDefaultWindow() { return DXRenderer::Get()->GetWindow(); }
		inline static DXInstance* GetDefaultInstance() { return DXRenderer::Get()->GetInstance(); }
		inline static DXDevice* GetDefaultDevice() { return DXRenderer::Get()->GetDevice(); }
		inline static DXSwapchain* GetDefaultSwapchain() { return DXRenderer::Get()->GetSwapchain(); }
		inline static DXRenderTarget* GetDefaultRTV() { return DXRenderer::Get()->GetRTV(); }
	}
}