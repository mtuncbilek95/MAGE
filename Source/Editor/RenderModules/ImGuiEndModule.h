#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Application/ApplicationModule.h"

namespace MAGE
{
	class GraphicsDevice;
	class Swapchain;
	class Fence;
	class GraphicsQueue;
	class CmdBuffer;
	class CmdPool;

	class ImGuiEndModule : public ApplicationModule
	{
	public:
		ImGuiEndModule();
		virtual ~ImGuiEndModule() override = default;

		virtual void Start() noexcept override;
		virtual void Update() noexcept override;
		virtual void Stop() noexcept override;

	protected:
		virtual b8 OnInitialize() noexcept override;
		virtual void OnPreUpdate() noexcept override;
		virtual void OnPostUpdate() noexcept override;

	private:
		GraphicsDevice* mDevice;
		Swapchain* mSwapchain;
		GraphicsQueue* mGraphicsQueue;

		SharedPtr<Fence> mFence;
		CmdBuffer* mCmdBuffer;
	};
}