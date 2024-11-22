#include "VQueue.h"

#include "../Core/VkAssert.h"
#include "../Device/VDevice.h"
#include "../Sync/VFence.h"
#include "../Sync/VSemaphore.h"
#include "../Command/VCmdBuffer.h"

namespace MAGE
{
	VQueue::VQueue(const QueueProps& desc, VDevice* device) : VkObject(device), m_props(desc)
	{
	}

	VQueue::~VQueue()
	{
	}

	void VQueue::Submit(const vector<VCmdBuffer*>& cmds, const vector<VSemaphore*>& waits, const vector<VSemaphore*>& signals, VFence* fence, VkPipelineStageFlags flags) const
	{
		vector<VkCommandBuffer> cmdBuffers(cmds.size());
		vector<VkSemaphore> waitSems(waits.size());
		vector<VkSemaphore> signalSems(signals.size());

		for (u32 i = 0; i < cmdBuffers.size(); i++)
			cmdBuffers[i] = cmds[i]->GetVkCmdBuffer();

		for (u32 i = 0; i < waitSems.size(); i++)
			waitSems[i] = waits[i]->GetVkSemaphore();

		for (u32 i = 0; i < signalSems.size(); i++)
			signalSems[i] = signals[i]->GetVkSemaphore();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = waitSems.size();
		submitInfo.pWaitSemaphores = waitSems.data();
		submitInfo.pWaitDstStageMask = &flags;
		submitInfo.commandBufferCount = cmdBuffers.size();
		submitInfo.pCommandBuffers = cmdBuffers.data();
		submitInfo.signalSemaphoreCount = signalSems.size();
		submitInfo.pSignalSemaphores = signalSems.data();

		ErrorUtils::VkAssert(vkQueueSubmit(m_props.m_queue, 1, &submitInfo, fence ? fence->GetVkFence() : VK_NULL_HANDLE), "VDevice");
	}

	void VQueue::Destroy()
	{
	}
}