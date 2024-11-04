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
#include "Engine/Rendering/RenderContext.h"

#include <thread>

namespace MAGE
{
	class TransferBatch : public Singleton<TransferBatch>
	{
	public:
		TransferBatch() = default;
		~TransferBatch();

		void Init(VQueue* pool);
		void RegisterCmdBuffer(VCmdBuffer* buffer);
		void ExecuteThemAll();

		VCmdPool* GetTransferPool() const { return &*m_transferPool; }

		void Destroy();

	private:
		bool HasPendingTransfer() const;

	private:
		Owned<VCmdPool> m_transferPool;
		Owned<VCmdBuffer> m_transferPrimary;
		Vector<VCmdBuffer*> m_refBuffers;
		Owned<VSemaphore> m_signal;

		mutable std::mutex m_mutex;
	};
}
