/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Objects/Singleton.h"

#include "Engine/Window/IndWindow.h"
#include "Engine/Vulkan/Instance/VInstance.h"
#include "Engine/Vulkan/Device/VDevice.h"
#include "Engine/Vulkan/Allocator/VAllocator.h"
#include "Engine/Vulkan/Queue/VQueue.h"
#include "Engine/Vulkan/Swapchain/VSwapchain.h"
#include "Engine/Vulkan/Sync/VSemaphore.h"
#include "Engine/Vulkan/Sync/VFence.h"
#include "Engine/Vulkan/Command/VCmdPool.h"
#include "Engine/Vulkan/Command/VCmdBuffer.h"

namespace MAGE
{
	class RenderContext final : public Singleton<RenderContext>
	{
	public:
		RenderContext() = default;
		~RenderContext() override final = default;

		void GenerateContext();
		void RegisterExecution(const voidFunc& exec, VCmdBuffer* buffer);

		void PrepareFrame();
		void ExecuteFrame(const vector<VCmdBuffer*>& buffers);
		void SubmitFrame();

		IndWindow* GetWindow() const { return &*m_window; }
		VInstance* GetInstance() const { return &*m_instance; }
		VDevice* GetDevice() const { return &*m_device; }
		VAllocator* GetAllocator() const { return &*m_alloc; }
		VSwapchain* GetSwapchain() const { return &*m_swapchain; }

	private:
		Owned<IndWindow> m_window;
		Owned<VInstance> m_instance;
		Owned<VDevice> m_device;
		Owned<VAllocator> m_alloc;

		Owned<VQueue> m_gQueue;
		Owned<VQueue> m_tQueue;
		Owned<VQueue> m_cQueue;

		Owned<VSwapchain> m_swapchain;

		vector<Owned<VSemaphore>> m_imageSemaphores;
		vector<Owned<VSemaphore>> m_presentSemaphores;
		vector<Owned<VFence>> m_inflightFences;

		Owned<VCmdPool> m_presentPool;
		vector<Owned<VCmdBuffer>> m_presentBuffers;

		u32 m_currentIndex = 0;
		u32 m_nextIndex = 0;

		vector<pair<VCmdBuffer*, voidFunc>> m_commands;
	};

	namespace Context
	{
		inline static IndWindow* GetMainWindow() { return RenderContext::Get().GetWindow(); }
		inline static VInstance* GetMainInstance() { return RenderContext::Get().GetInstance(); }
		inline static VDevice* GetMainDevice() { return RenderContext::Get().GetDevice(); }
		inline static VAllocator* GetMainAllocator() { return RenderContext::Get().GetAllocator(); }
	}
}
