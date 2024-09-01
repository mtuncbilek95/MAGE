#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Object/IObject.h"

#include "imgui.h"

#include <vulkan/vulkan.h>

namespace MAGE
{
	class Fence;
	class GraphicsQueue;
	class CmdPool;
	class CmdBuffer;
	class DescriptorPool;

	class ImGuiRenderer : public IObject
	{
	public:
		ImGuiRenderer();
		~ImGuiRenderer();

		CmdBuffer* GetCmdBuffer() { return mCmdBuffer.get(); }
		CmdPool* GetCmdPool() { return mCmdPool.get(); }
		GraphicsQueue* GetGraphicsQueue() { return mGraphicsQueue; }

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
