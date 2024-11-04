#include "TransferBatch.h"

namespace MAGE
{
	Gfx::Context& m_context = Gfx::Context::Get();

	TransferBatch::~TransferBatch()
	{
		Destroy();
	}

	void TransferBatch::Init(VQueue* queue)
	{
		CmdPoolProps prop = {};
		prop.queue = queue;
		prop.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		m_transferPool = MakeOwned<VCmdPool>(prop, m_context.GetDevice());
		m_transferPrimary = m_transferPool->CreateCmdBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		m_signal = m_context.GetDevice()->CreateSyncSemaphore();
	}

	void TransferBatch::RegisterCmdBuffer(VCmdBuffer* buffer)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (!m_transferPrimary)
			m_transferPrimary = m_transferPool->CreateCmdBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		m_refBuffers.push_back(buffer);
	}

	void TransferBatch::ExecuteThemAll()
	{
		while (HasPendingTransfer())
		{
			std::lock_guard<std::mutex> lock(m_mutex);

			m_transferPrimary->BeginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
			m_transferPrimary->ExecuteCommands(m_refBuffers);
			m_transferPrimary->EndRecording();

			m_context.GetDevice()->SubmitQueue(m_context.GetTransferQueue(), &*m_transferPrimary, nullptr, &*m_signal, nullptr, VK_PIPELINE_STAGE_TRANSFER_BIT);

			m_refBuffers.clear();
		}
	}

	void TransferBatch::Destroy()
	{
		m_signal->Destroy();
		m_transferPrimary->Destroy();
		m_transferPool->Destroy();
	}

	bool TransferBatch::HasPendingTransfer() const
	{
		return !m_refBuffers.empty();
	}
}
