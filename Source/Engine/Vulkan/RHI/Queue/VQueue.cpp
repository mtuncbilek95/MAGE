#include "VQueue.h"

#include "../Device/VDevice.h"
#include "../Command/VCmdBuffer.h"
#include "../Sync/VSemaphore.h"
#include "../Sync/VFence.h"

#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	VQueue::VQueue(const QueueProps& desc, vk::Queue queue, VDevice* device) : VkObject(device), m_props(desc), m_queue(queue) {}
	VQueue::~VQueue() { Destroy(); }
	
	void VQueue::Submit(const Vector<VCmdBuffer*>& cmdBuffers, const Vector<VSemaphore*>& signals, const Vector<VSemaphore*>& waits, const Vector<vk::PipelineStageFlags>& dstFlags, VFence* fence) const
	{
		Vector<vk::CommandBuffer> cmds(cmdBuffers.size(), VK_NULL_HANDLE);
		Vector<vk::Semaphore> signalSems(signals.size(), VK_NULL_HANDLE);
		Vector<vk::Semaphore> waitSems(waits.size(), VK_NULL_HANDLE);

		for (u32 i = 0; i < cmds.size(); i++)
			cmds[i] = cmdBuffers[i]->GetVkCmdBuffer();

		for (u32 i = 0; i < signalSems.size(); i++)
			signalSems[i] = signals[i]->GetVkSemaphore();

		for (u32 i = 0; i < waitSems.size(); i++)
			waitSems[i] = waits[i]->GetVkSemaphore();

		vk::SubmitInfo info = {};
		info.commandBufferCount = cmdBuffers.size();
		info.pCommandBuffers = cmds.data();
		info.waitSemaphoreCount = waits.size();
		info.pWaitSemaphores = waitSems.data();
		info.signalSemaphoreCount = signals.size();
		info.pSignalSemaphores = signalSems.data();
		info.pWaitDstStageMask = dstFlags.data();

		ErrorUtils::VkAssert(m_queue.submit(1, &info, fence ? fence->GetVkFence() : vk::Fence()), "VSemaphore");
	}

	void VQueue::Destroy() {}
}
