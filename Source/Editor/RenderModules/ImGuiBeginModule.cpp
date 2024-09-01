#include "ImGuiBeginModule.h"

#include "Engine/Graphics/API/GraphicsAPI.h"
#include "Engine/Graphics/Device/GraphicsDevice.h"
#include "Engine/Graphics/Swapchain/Swapchain.h"
#include "Engine/Graphics/Queue/GraphicsQueue.h"
#include "Engine/Graphics/Command/CmdBuffer.h"
#include "Engine/Graphics/Command/CmdPool.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include "Editor/API/ImGuiAPI.h"

namespace MAGE
{
	ImGuiBeginModule::ImGuiBeginModule()
	{
		SetModuleName("ImGuiBeginModule");
	}

	void ImGuiBeginModule::Start() noexcept
	{
		mCmdBuffer->BeginRecording();

		for (u8 i = 0; i < mSwapchain->GetBufferCount(); ++i)
		{
			TextureImage* image = mSwapchain->GetImage(i);

			TextureImageBarrier barrier = {};
			barrier.ArrayIndex = 0;
			barrier.MipIndex = 0;
			barrier.SourceAccessMask = GraphicsMemoryAccessFlags::GMAF_Unknown;
			barrier.DestinationAccessMask = GraphicsMemoryAccessFlags::GMAF_ColorAttachmentRead;
			barrier.SourceQueue = GraphicsQueueType::GQT_Graphics;
			barrier.DestinationQueue = GraphicsQueueType::GQT_Graphics;
			barrier.OldLayout = TextureLayout::TL_Undefined;
			barrier.NewLayout = TextureLayout::TL_Present;
			barrier.SourceStageFlags = PipelineStageFlags::PSF_TopOfPipe;
			barrier.DestinationStageFlags = PipelineStageFlags::PSF_ColorAttachmentOutput;

			barrier.AspectMask = TextureAspectFlags::TAF_ColorAspect;

			mCmdBuffer->SetTextureBarrier(image, barrier);
		}

		mCmdBuffer->EndRecording();

		PipelineStageFlags waitStage = PipelineStageFlags::PSF_ColorAttachmentOutput;
		mDevice->SubmitQueue(mGraphicsQueue, mCmdBuffer, 1, nullptr, 0, nullptr, 0, mFence.get(), &waitStage);

		mDevice->WaitFence(mFence.get());
		mDevice->ResetFence(mFence.get());
	}

	void ImGuiBeginModule::Update() noexcept
	{
		if(mSwapchain->GetImageSize().x == 0 || mSwapchain->GetImageSize().y == 0)
			return;

		u32 index = mSwapchain->AcquireNextImage(mFence.get(), nullptr);
		mDevice->WaitFence(mFence.get());
		mDevice->ResetFence(mFence.get());

		mCmdBuffer->BeginRecording();

		TextureImage* image = mSwapchain->GetImage(index);

		TextureImageBarrier barrier = {};
		barrier.ArrayIndex = 0;
		barrier.MipIndex = 0;
		barrier.SourceAccessMask = GraphicsMemoryAccessFlags::GMAF_ColorAttachmentRead;
		barrier.DestinationAccessMask = GraphicsMemoryAccessFlags::GMAF_ColorAttachmentWrite;
		barrier.SourceQueue = GraphicsQueueType::GQT_Graphics;
		barrier.DestinationQueue = GraphicsQueueType::GQT_Graphics;
		barrier.OldLayout = TextureLayout::TL_Present;
		barrier.NewLayout = TextureLayout::TL_ColorAttachment;
		barrier.SourceStageFlags = PipelineStageFlags::PSF_ColorAttachmentOutput;
		barrier.DestinationStageFlags = PipelineStageFlags::PSF_ColorAttachmentOutput;

		barrier.AspectMask = TextureAspectFlags::TAF_ColorAspect;

		mCmdBuffer->SetTextureBarrier(image, barrier);

		DynamicPassAttachmentDesc passColorAttachmentDesc = {};
		passColorAttachmentDesc.ImageBuffer = mSwapchain->GetImageView(index);
		passColorAttachmentDesc.ImageLayout = TextureLayout::TL_ColorAttachment;
		passColorAttachmentDesc.LoadOperation = AttachmentLoadOperation::ALO_Clear;
		passColorAttachmentDesc.StoreOperation = AttachmentStoreOperation::ASO_Store;
		passColorAttachmentDesc.ClearColor = { 0.1f, 0.2f, 0.3f, 1.0f };

		DynamicPassDesc passDesc = {};
		passDesc.ColorAttachments = { passColorAttachmentDesc };
		passDesc.layerCount = 1;
		passDesc.Extent = { mSwapchain->GetImageSize().x, mSwapchain->GetImageSize().y};
		passDesc.Offset = { 0, 0 };
		passDesc.viewMask = 0;

		mCmdBuffer->BeginRendering(passDesc);

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiBeginModule::Stop() noexcept
	{
	}

	b8 ImGuiBeginModule::OnInitialize() noexcept
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
		if (!mFence)
			return false;

		CmdPoolDesc poolDesc = {};
		poolDesc.PoolType = CmdPoolType::CPT_Graphics;

		mCmdBuffer = ImGuiAPI::GetAPI()->GetEditorRenderer()->GetCmdBuffer();

		return true;
	}

	void ImGuiBeginModule::OnPreUpdate() noexcept
	{
	}

	void ImGuiBeginModule::OnPostUpdate() noexcept
	{
	}
}
