#include "GraphicsModule.h"

#include <Engine/Graphics/API/GraphicsAPI.h>
#include <Engine/Window/WindowAPI.h>

#include <Engine/Graphics/Device/GraphicsDevice.h>
#include <Engine/Graphics/Swapchain/Swapchain.h>
#include <Engine/Graphics/Command/CmdBuffer.h>

namespace MAGE
{
	GraphicsModule::GraphicsModule()
	{
		SetModuleName("GraphicsModule");
		mGraphicsAPI = GraphicsAPI::GetAPI();
	}

	void GraphicsModule::Start() noexcept
	{
	}

	void GraphicsModule::Update() noexcept
	{
		if(WindowAPI::GetAPI()->GetDefaultWindow()->GetWindowResolution().x != mGraphicsAPI->GetSwapchain()->GetImageSize().x ||
			WindowAPI::GetAPI()->GetDefaultWindow()->GetWindowResolution().y != mGraphicsAPI->GetSwapchain()->GetImageSize().y)
		{
			mGraphicsAPI->GetSwapchain()->ResizeSwapchain(WindowAPI::GetAPI()->GetDefaultWindow()->GetWindowResolution());

			if (WindowAPI::GetAPI()->GetDefaultWindow()->GetWindowResolution().x == 0 || WindowAPI::GetAPI()->GetDefaultWindow()->GetWindowResolution().y == 0)
				return;

			const auto fence = GraphicsAPI::GetAPI()->GetDevice()->CreateGraphicsFence(false);
			const auto cmdPool = GraphicsAPI::GetAPI()->GetDevice()->CreateCommandPool({ CmdPoolType::Graphics });
			const auto cmdBuffer = GraphicsAPI::GetAPI()->GetDevice()->CreateCommandBuffer({ cmdPool.get()});

			cmdBuffer->BeginRecording();

			for (u8 i = 0; i < mGraphicsAPI->GetSwapchain()->GetBufferCount(); ++i)
			{
				TextureImage* image = mGraphicsAPI->GetSwapchain()->GetImage(i);

				TextureImageBarrier barrier = {};
				barrier.ArrayIndex = 0;
				barrier.MipIndex = 0;
				barrier.SourceAccessMask = GraphicsMemoryAccessFlags::Unknown;
				barrier.DestinationAccessMask = GraphicsMemoryAccessFlags::ColorAttachmentRead;
				barrier.SourceQueue = GraphicsQueueType::Graphics;
				barrier.DestinationQueue = GraphicsQueueType::Graphics;
				barrier.OldLayout = TextureLayout::Undefined;
				barrier.NewLayout = TextureLayout::Present;
				barrier.SourceStageFlags = PipelineStageFlags::TopOfPipe;
				barrier.DestinationStageFlags = PipelineStageFlags::ColorAttachmentOutput;

				barrier.AspectMask = TextureAspectFlags::ColorAspect;

				cmdBuffer->SetTextureBarrier(image, barrier);
			}

			cmdBuffer->EndRecording();

			auto waitStage = PipelineStageFlags::ColorAttachmentOutput;
			mGraphicsAPI->GetDevice()->SubmitQueue(mGraphicsAPI->GetGraphicsQueue(), cmdBuffer.get(), 1, nullptr, 0, nullptr, 0, fence.get(), &waitStage);

			mGraphicsAPI->GetDevice()->WaitFence(fence.get());
			mGraphicsAPI->GetDevice()->ResetFence(fence.get());
		}
	}

	void GraphicsModule::Stop() noexcept
	{
		mGraphicsAPI->GetDevice()->WaitIdle();
	}

	b8 GraphicsModule::OnInitialize() noexcept
	{
		GraphicsInstanceDesc instanceDesc = {};
		instanceDesc.AppName = "Editor"; // TODO: Get from config
		instanceDesc.EngineName = "MAGE"; // TODO: Get from config
		instanceDesc.AppVersion = Vec3b(1, 0, 0); // TODO: Get from config
		instanceDesc.EngineVersion = Vec3b(1, 0, 0); // TODO: Get from config

		if (const auto tempRefInstance = mGraphicsAPI->CreateInstance(instanceDesc); tempRefInstance == nullptr)
			return false;

		if (const auto tempRefDevice = mGraphicsAPI->CreateDevice(); tempRefDevice == nullptr)
			return false;

		SwapchainDesc swapchainDesc = {};
		swapchainDesc.BufferCount = 2;
#if defined(MAGE_WINDOWS)
		swapchainDesc.ImageFormat = TextureFormat::RGBA8_UNorm;
#elif defined(MAGE_LINUX)
		swapchainDesc.ImageFormat = TextureFormat::BGRA8_UNorm;
#endif
		swapchainDesc.TextureUsage = TextureUsageFlags::ColorAttachment;
		swapchainDesc.VSync = PresentMode::FullVSync;
		swapchainDesc.ImageSize = WindowAPI::GetAPI()->GetDefaultWindow()->GetWindowResolution();
		swapchainDesc.pRequestQueue = mGraphicsAPI->GetGraphicsQueue();

		if (const auto tempRefSwapchain = mGraphicsAPI->CreateSwapchain(swapchainDesc); tempRefSwapchain == nullptr)
			return false;

		return true;
	}

	void GraphicsModule::OnPreUpdate() noexcept
	{
	}

	void GraphicsModule::OnPostUpdate() noexcept
	{
	}
}
