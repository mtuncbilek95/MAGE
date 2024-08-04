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

	GraphicsModule::~GraphicsModule()
	{
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

			auto fence = GraphicsAPI::GetAPI()->GetDevice()->CreateGraphicsFence(false);
			auto cmdPool = GraphicsAPI::GetAPI()->GetDevice()->CreateCommandPool({ CmdPoolType::Graphics });
			auto cmdBuffer = GraphicsAPI::GetAPI()->GetDevice()->CreateCommandBuffer({ cmdPool.get()});

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

			PipelineStageFlags waitStage = PipelineStageFlags::ColorAttachmentOutput;
			mGraphicsAPI->GetDevice()->SubmitQueue(mGraphicsAPI->GetGraphicsQueue(), cmdBuffer.get(), 1, nullptr, 0, nullptr, 0, fence.get(), &waitStage);

			mGraphicsAPI->GetDevice()->WaitFence(fence.get());
			mGraphicsAPI->GetDevice()->ResetFence(fence.get());
		}
	}

	void GraphicsModule::Stop() noexcept
	{
		mGraphicsAPI->GetDevice()->WaitIdle();
	}

	bool GraphicsModule::OnInitialize() noexcept
	{
		GraphicsInstanceDesc instanceDesc = {};
		instanceDesc.AppName = "Editor"; // TODO: Get from config
		instanceDesc.EngineName = "MAGE"; // TODO: Get from config
		instanceDesc.AppVersion = Vec3b(1, 0, 0); // TODO: Get from config
		instanceDesc.EngineVersion = Vec3b(1, 0, 0); // TODO: Get from config

		auto tempRefInstance = mGraphicsAPI->CreateInstance(instanceDesc);
		if (tempRefInstance == nullptr)
			return false;

		auto tempRefDevice = mGraphicsAPI->CreateDevice();
		if (tempRefDevice == nullptr)
			return false;

		SwapchainDesc swapchainDesc = {};
		swapchainDesc.BufferCount = 2; // TODO: Get from config
		swapchainDesc.ImageFormat = TextureFormat::RGBA8_UNorm; // TODO: Get from config
		swapchainDesc.TextureUsage = TextureUsageFlags::ColorAttachment; // TODO: Get from config
		swapchainDesc.VSync = PresentMode::FullVSync; // TODO: Get from config
		swapchainDesc.ImageSize = WindowAPI::GetAPI()->GetDefaultWindow()->GetWindowResolution();
		swapchainDesc.pRequestQueue = mGraphicsAPI->GetGraphicsQueue();

		auto tempRefSwapchain = mGraphicsAPI->CreateSwapchain(swapchainDesc);
		if (tempRefSwapchain == nullptr)
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
