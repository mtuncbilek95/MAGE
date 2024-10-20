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

#include "Engine/VulkanRHI/Instance/VInstance.h"
#include "Engine/VulkanRHI/Device/VDevice.h"
#include "Engine/VulkanRHI/Queue/VQueue.h"
#include "Engine/VulkanRHI/Swapchain/VSwapchain.h"
#include "Engine/VulkanRHI/Sync/VSemaphore.h"
#include "Engine/VulkanRHI/Sync/VFence.h"
#include "Engine/VulkanRHI/Command/VCmdPool.h"
#include "Engine/VulkanRHI/Command/VCmdBuffer.h"

namespace MAGE
{
	namespace Gfx
	{
		class Context : public Singleton<Context>
		{
		public:
			Context() = default;
			~Context() = default;

			void Init();
			void PrepareFrame();
			void SubmitFrame();
			void Shutdown();

			VInstance* GetInstance() const { return &*m_instance; }
			VDevice* GetDevice() const { return &*m_device; }
			VSwapchain* GetSwapchain() const { return &*m_swapchain; }
			VQueue* GetGraphicsQueue() const { return &*m_graphicsQueue; }
			VQueue* GetComputeQueue() const { return &*m_computeQueue; }
			VQueue* GetTransferQueue() const { return &*m_transferQueue; }
			VCmdBuffer* GetCmdBuffer() const { return &*m_commandBuffers[m_reqImIndex]; }

		private:
			Owned<VInstance> m_instance;
			Owned<VDevice> m_device;
			
			Owned<VQueue> m_graphicsQueue;
			Owned<VQueue> m_computeQueue;
			Owned<VQueue> m_transferQueue;

			Owned<VSwapchain> m_swapchain;
			
			Vector<Owned<VSemaphore>> m_renderSemaphores;
			Owned<VFence> m_acquireFence;

			Owned<VCmdPool> m_commandPool;
			Vector<Owned<VCmdBuffer>> m_commandBuffers;

			u32 m_reqImIndex = 0;
		};
	}
}
