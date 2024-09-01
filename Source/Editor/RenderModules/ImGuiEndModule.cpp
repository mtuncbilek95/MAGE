#include "ImGuiEndModule.h"

#include "Engine/Graphics/API/GraphicsAPI.h"
#include "Engine/Graphics/Device/GraphicsDevice.h"
#include "Engine/Graphics/Swapchain/Swapchain.h"
#include "Engine/Graphics/Queue/GraphicsQueue.h"
#include "Engine/Graphics/Command/CmdBuffer.h"
#include "Engine/Graphics/Command/CmdPool.h"

#include "Engine/Vulkan/Command/VCmdBuffer.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include "Editor/API/ImGuiAPI.h"

namespace MAGE
{
	ImGuiEndModule::ImGuiEndModule()
	{
		SetModuleName("ImGuiEndModule");
	}

	void ImGuiEndModule::Start() noexcept
	{
	}

	void ImGuiEndModule::Update() noexcept
	{
		if (mSwapchain->GetImageSize().x == 0 || mSwapchain->GetImageSize().y == 0)
			return;

		ImGui::Render();

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), mCmdBuffer->GetAs<VCmdBuffer>()->GetVkCmdBuffer());
	}

	void ImGuiEndModule::Stop() noexcept
	{
	}

	b8 ImGuiEndModule::OnInitialize() noexcept
	{
		mDevice = GraphicsAPI::GetAPI()->GetDevice();

		if (mDevice == nullptr)
			return false;

		mSwapchain = GraphicsAPI::GetAPI()->GetSwapchain();

		if (mSwapchain == nullptr)
			return false;

		mGraphicsQueue = GraphicsAPI::GetAPI()->GetGraphicsQueue();
		if (mGraphicsQueue == nullptr)
			return false;

		mFence = mDevice->CreateGraphicsFence(false);
		if (mFence == nullptr)
			return false;

		mCmdBuffer = ImGuiAPI::GetAPI()->GetEditorRenderer()->GetCmdBuffer();

		return true;
	}

	void ImGuiEndModule::OnPreUpdate() noexcept
	{
	}

	void ImGuiEndModule::OnPostUpdate() noexcept
	{
		if (mSwapchain->GetImageSize().x == 0 || mSwapchain->GetImageSize().y == 0)
			return;

		mCmdBuffer->EndRendering();

		u32 index = mSwapchain->GetImageIndex();

		TextureImageBarrier postRenderBarrier = {};
		postRenderBarrier.ArrayIndex = 0;
		postRenderBarrier.MipIndex = 0;
		postRenderBarrier.SourceAccessMask = GraphicsMemoryAccessFlags::GMAF_ColorAttachmentWrite;
		postRenderBarrier.SourceQueue = GraphicsQueueType::GQT_Graphics;
		postRenderBarrier.SourceStageFlags = PipelineStageFlags::PSF_ColorAttachmentOutput;
		postRenderBarrier.OldLayout = TextureLayout::TL_ColorAttachment;

		postRenderBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::GMAF_MemoryRead;
		postRenderBarrier.DestinationQueue = GraphicsQueueType::GQT_Graphics;
		postRenderBarrier.DestinationStageFlags = PipelineStageFlags::PSF_BottomOfPipe;
		postRenderBarrier.NewLayout = TextureLayout::TL_Present;

		postRenderBarrier.AspectMask = TextureAspectFlags::TAF_ColorAspect;

		mCmdBuffer->SetTextureBarrier(mSwapchain->GetImage(index), postRenderBarrier);

		mCmdBuffer->EndRecording();

		auto flag = PipelineStageFlags::PSF_ColorAttachmentOutput;
		mDevice->SubmitQueue(mGraphicsQueue, mCmdBuffer, 1, nullptr, 0, nullptr, 0, mFence.get(), &flag);
		mDevice->WaitFence(mFence.get());
		mDevice->ResetFence(mFence.get());

		mSwapchain->Present(nullptr);
	}
}
