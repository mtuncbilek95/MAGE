/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Helpers/Singleton.h"

#include "Engine/VulkanGraphics/Instance/VulkanInstance.h"
#include "Engine/VulkanGraphics/Device/VulkanDevice.h"
#include "Engine/VulkanGraphics/Queue/VulkanQueue.h"
#include "Engine/VulkanGraphics/Swapchain/VulkanSwapchain.h"
#include "Engine/VulkanGraphics/Sync/VulkanSemaphore.h"
#include "Engine/VulkanGraphics/Sync/VulkanFence.h"
#include "Engine/VulkanGraphics/Command/VulkanCmdPool.h"
#include "Engine/VulkanGraphics/Command/VulkanCmdBuffer.h"

namespace MAGE
{
	namespace Gfx
	{
		class RendererContext : public Singleton<RendererContext>
		{
		public:
			RendererContext() = default;
			~RendererContext() = default;

			void Init();
			void PrepareFrame();
			void SubmitFrame();
			void Shutdown();

			VulkanInstance* GetInstance() const { return &*m_instance; }
			VulkanDevice* GetDevice() const { return &*m_device; }
			VulkanSwapchain* GetSwapchain() const { return &*m_swapchain; }
			VulkanQueue* GetGraphicsQueue() const { return &*m_graphicsQueue; }
			VulkanQueue* GetComputeQueue() const { return &*m_computeQueue; }
			VulkanQueue* GetTransferQueue() const { return &*m_transferQueue; }

			VulkanCmdBuffer* GetCmdBuffer() const { return &*m_commandBuffers[m_reqImIndex]; }

		private:
			Owned<VulkanInstance> m_instance;
			Owned<VulkanDevice> m_device;
			Owned<VulkanSwapchain> m_swapchain;

			Owned<VulkanQueue> m_graphicsQueue;
			Owned<VulkanQueue> m_computeQueue;
			Owned<VulkanQueue> m_transferQueue;

			Vector<Owned<VulkanSemaphore>> m_renderSemaphores;
			Owned<VulkanFence> m_acquireFence;

			Owned<VulkanCmdPool> m_commandPool;
			Vector<Owned<VulkanCmdBuffer>> m_commandBuffers;

			u32 m_reqImIndex = 0;
		};
	}
}
