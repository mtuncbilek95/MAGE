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
		Vector<vk::CommandBuffer> cmds(cmdBuffers.size());
		Vector<vk::Semaphore> signalSems(signals.size());
		Vector<vk::Semaphore> waitSems(waits.size());

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

		ErrorUtils::VkAssert(m_queue.submit(1, &info, fence ? fence->GetVkFence() : vk::Fence()), "VQueue - Submit Multi");
	}

	void VQueue::Submit(VCmdBuffer* cmdBuffer, VSemaphore* signal, VSemaphore* wait, vk::PipelineStageFlags dstFlag, VFence* fence) const
	{
		vk::CommandBuffer cmd = cmdBuffer->GetVkCmdBuffer();
		vk::Semaphore sig = signal ? signal->GetVkSemaphore() : nullptr;
		vk::Semaphore waitSem = wait ? wait->GetVkSemaphore() : nullptr;

		vk::SubmitInfo info = {};
		info.commandBufferCount = 1;
		info.pCommandBuffers = &cmd;
		info.waitSemaphoreCount = wait ? 1 : 0;
		info.pWaitSemaphores = &waitSem;
		info.signalSemaphoreCount = signal ? 1 : 0;
		info.pSignalSemaphores = &sig;
		info.pWaitDstStageMask = &dstFlag;

		ErrorUtils::VkAssert(m_queue.submit(1, &info, fence ? fence->GetVkFence() : vk::Fence()), "VQueue - Submit Single");
	}

	void VQueue::Destroy() {}
}
