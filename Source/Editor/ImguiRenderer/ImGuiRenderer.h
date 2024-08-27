#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/IObject.h>

#include "imgui.h"

#include <vulkan/vulkan.h>

namespace MAGE
{
	class Fence;
	class GraphicsQueue;
	class CmdPool;
	class CmdBuffer;
	class DescriptorPool;

	class ImGuiRenderer final : public IObject
	{
	public:
		ImGuiRenderer();
		~ImGuiRenderer() override;

		CmdBuffer* GetCmdBuffer() const { return mCmdBuffer.get(); }
		CmdPool* GetCmdPool() const { return mCmdPool.get(); }
		GraphicsQueue* GetGraphicsQueue() const { return mGraphicsQueue; }

	private:
		void InitVk();

	private:
		ImGuiContext* mContext;

		SharedPtr<DescriptorPool> mDescriptorPool;

		GraphicsQueue* mGraphicsQueue;
		SharedPtr<Fence> mFence;

		SharedPtr<CmdBuffer> mCmdBuffer;
		SharedPtr<CmdPool> mCmdPool;
	};
}
