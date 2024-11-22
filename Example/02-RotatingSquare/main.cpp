#include <Engine/Core/Core.h>
#include <Engine/Window/IndWindow.h>
#include <Engine/Vulkan/Instance/VInstance.h>
#include <Engine/Vulkan/Device/VDevice.h>
#include <Engine/Vulkan/Allocator/VAllocator.h>
#include <Engine/Vulkan/Queue/VQueue.h>
#include <Engine/Vulkan/Swapchain/VSwapchain.h>
#include <Engine/Vulkan/Command/VCmdPool.h>
#include <Engine/Vulkan/Command/VCmdBuffer.h>
#include <Engine/Vulkan/Sync/VFence.h>
#include <Engine/Vulkan/Sync/VSemaphore.h>
#include <Engine/Vulkan/Shader/VShader.h>
#include <Engine/Vulkan/Descriptor/VDescLayout.h>
#include <Engine/Vulkan/Descriptor/VDescPool.h>
#include <Engine/Vulkan/Descriptor/VDescSet.h>
#include <Engine/Vulkan/Pipeline/VPipeline.h>
#include <Engine/Vulkan/Buffer/VBuffer.h>

using namespace MAGE;

struct Vertex
{
	Math::Vec3f pos;
	Math::Vec4f color;
};

vector<Vertex> square =
{
	{{ -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
	{{ -0.5f,  0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
	{{  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }},
	{{  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }}
};

vector<u32> indices = { 0, 1, 3, 1, 2, 3 };

struct MVPStruct
{
	Math::Mat4f model;
	Math::Mat4f view;
	Math::Mat4f proj;
};

MVPStruct testmvp = { Math::Mat4f(1.0f), Math::Mat4f(1.0f), Math::Mat4f(1.0f) };

void DoMVP()
{
	testmvp.model = glm::translate(testmvp.model, glm::vec3(0.f, 0.f, 0.f));
	testmvp.model = glm::rotate(testmvp.model, glm::radians(0.f), glm::vec3(1.0f, 0.0f, 0.0f));
	testmvp.model = glm::rotate(testmvp.model, glm::radians(0.f), glm::vec3(0.0f, 1.0f, 0.0f));
	testmvp.model = glm::rotate(testmvp.model, glm::radians(1.f), glm::vec3(0.0f, 0.0f, 1.0f));
	testmvp.proj = glm::perspective(glm::radians(74.0f), 1600.f / 900.f, 0.1f, 100.f);
	testmvp.view = glm::lookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	testmvp.proj[1][1] *= -1;
}

int main(int argC, char** argV)
{
	WindowProps windProp =
	{
		.windowTitle = "Hello Triangle",
		.windowSize = {1600, 900},
		.isWindowed = true
	};
	IndWindow window(windProp);
	InstanceProps gInsProp =
	{
		.appName = "Hello Triangle",
		.engineName = "MAGE::Delusion",
		.appVersion = {1, 0, 0},
		.engineVersion = {1, 0, 0}
	};
	Owned<VInstance> gInstance = MakeOwned<VInstance>(gInsProp);

	DeviceProps gDevProp =
	{
		.graphicsQueueCount = 1,
		.computeQueueCount = 1,
		.transferQueueCount = 1
	};
	Owned<VDevice> gDevice = MakeOwned<VDevice>(gDevProp, &*gInstance);
	Owned<VAllocator> gAlloc = MakeOwned<VAllocator>(&*gDevice);

	Owned<VQueue> gQueue = gDevice->CreateQueue(VK_QUEUE_GRAPHICS_BIT);
	Owned<VQueue> tQueue = gDevice->CreateQueue(VK_QUEUE_TRANSFER_BIT);
	Owned<VQueue> cQueue = gDevice->CreateQueue(VK_QUEUE_COMPUTE_BIT);

	SwapchainProps swapProp =
	{
		.imageSize = { 1600, 900 },
		.imageCount = 3,
		.format = VK_FORMAT_R8G8B8A8_UNORM,
		.presentMode = VK_PRESENT_MODE_FIFO_KHR,
		.graphicsQueue = &*gQueue,
		.usedWindow = &window
	};
	Owned<VSwapchain> gSwapchain = MakeOwned<VSwapchain>(swapProp, &*gDevice);

	CmdPoolProps gPoolProps =
	{
		.queue = &*gQueue,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
	};
	Owned<VCmdPool> gPool = MakeOwned<VCmdPool>(gPoolProps, &*gDevice);

	vector<Owned<VCmdBuffer>> gPrimBuffers;
	vector<Owned<VSemaphore>> gRenderSemaphores;

	for (u32 i = 0; i < gSwapchain->GetImageCount(); i++)
	{
		gPrimBuffers.push_back(gPool->CreateCmdBuffer());
		gRenderSemaphores.push_back(MakeOwned<VSemaphore>(&*gDevice));
	}
	Owned<VFence> gImgFence = MakeOwned<VFence>(false, &*gDevice);

	ShaderProps shaderProp = {};
	shaderProp.shaderPath = R"(D:\Projects\MAGE\Shaders\RotatingSquare.vert)";
	shaderProp.shaderStage = VK_SHADER_STAGE_VERTEX_BIT;
	Owned<VShader> vShader = MakeOwned<VShader>(shaderProp, &*gDevice);

	shaderProp.shaderPath = R"(D:\Projects\MAGE\Shaders\RotatingSquare.frag)";
	shaderProp.shaderStage = VK_SHADER_STAGE_FRAGMENT_BIT;
	Owned<VShader> fShader = MakeOwned<VShader>(shaderProp, &*gDevice);

	DescLayoutProps descLayoutProp = {};
	descLayoutProp.bindings = { { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT } };

	Owned<VDescLayout> uniformLayout = MakeOwned<VDescLayout>(descLayoutProp, &*gDevice);

	DescPoolProps descPoolProp = {};
	descPoolProp.maxSets = 1;
	descPoolProp.poolSizes = { {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1} };

	Owned<VDescPool> uniformPool = MakeOwned<VDescPool>(descPoolProp, &*gDevice);

	DescSetProps setProp = { &*uniformLayout, &*uniformPool };
	Owned<VDescSet> uniformSet = MakeOwned<VDescSet>(setProp, &*gDevice);

	GraphicsPipelineProps pipeProp = {};
	pipeProp.layouts = { &*uniformLayout };
	pipeProp.shaderStages = { &*vShader, &*fShader };
	pipeProp.viewportState.viewport = vk::Viewport(0, 0, 1600, 900, 0.0f, 1.0f);
	pipeProp.viewportState.scissor = vk::Rect2D({ 0,0 }, { 1600, 900 });
	pipeProp.inputAssembler.bindings = { {VK_VERTEX_INPUT_RATE_VERTEX, {VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32B32A32_SFLOAT}} };
	pipeProp.dynamicRendering.colorAttachments = { VK_FORMAT_R8G8B8A8_UNORM };
	pipeProp.blendState.attachments = { BlendStateAttachment() };

	Owned<VPipeline> pipeline = MakeOwned<VPipeline>(pipeProp, &*gDevice);

	BufferProps bufferProp = {};
	bufferProp.sizeInBytes = sizeof(Vertex) * square.size() + indices.size() * sizeof(u32);
	bufferProp.usageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	Owned<VBuffer> vBuffer = MakeOwned<VBuffer>(bufferProp, &*gDevice);
	vBuffer->BindMemory(gAlloc->GetAvailableMemory(AllocProps(vBuffer->GetRequestedSize(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)));
	vBuffer->Update({ square.data(), sizeof(Vertex) * square.size() }, 0);
	vBuffer->Update({ indices.data(), indices.size() * sizeof(u32) }, sizeof(Vertex) * square.size());

	bufferProp.sizeInBytes = sizeof(MVPStruct);
	bufferProp.usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	Owned<VBuffer> uBuffer = MakeOwned<VBuffer>(bufferProp, &*gDevice);
	uBuffer->BindMemory(gAlloc->GetAvailableMemory(AllocProps(uBuffer->GetRequestedSize(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)));
	uBuffer->Update({ &testmvp, sizeof(MVPStruct) }, 0);

	SetUpdateProps uUpdateProp = {};
	uUpdateProp.entries = { {&*uBuffer, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 0, 0, 0} };
	uniformSet->UpdateData(uUpdateProp);

	window.Show();
	while (!window.IsClosed())
	{
		window.PollEvents();
		DoMVP();
		uBuffer->Update({ &testmvp, sizeof(MVPStruct) }, 0);

		u32 imgIndex = gSwapchain->AcquireNextImage(nullptr, &*gImgFence);
		gImgFence->Wait();
		gImgFence->Reset();
		gPrimBuffers[imgIndex]->BeginRecord(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		ImageBarrierProps barrier = {};
		barrier.srcPipeline = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		barrier.dstPipeline = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		barrier.image = gSwapchain->GetCurrentImage();
		gPrimBuffers[imgIndex]->ImageBarrier(barrier);

		RenderProps renderProp = {};
		renderProp.colorAttachment = { { gSwapchain->GetCurrentView(), VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR,
			VK_ATTACHMENT_STORE_OP_STORE, {.39f, .58f, .92f, 1.f}, {1.f, 0}} };
		renderProp.extent = { gSwapchain->GetImageSize().x, gSwapchain->GetImageSize().y };
		renderProp.layerCount = 1;
		renderProp.viewMask = 0;
		gPrimBuffers[imgIndex]->BeginRendering(renderProp);
		gPrimBuffers[imgIndex]->BindPipeline(&*pipeline);
		gPrimBuffers[imgIndex]->BindDescriptors({ &*uniformSet });

		gPrimBuffers[imgIndex]->BindVertexBuffer({ &*vBuffer });
		gPrimBuffers[imgIndex]->BindIndexBuffer(&*vBuffer, sizeof(Vertex) * square.size());
		gPrimBuffers[imgIndex]->DrawIndexed(indices.size(), 0, 0, 0, 1);
		gPrimBuffers[imgIndex]->EndRendering();

		barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
		barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		gPrimBuffers[imgIndex]->ImageBarrier(barrier);
		gPrimBuffers[imgIndex]->EndRecord();

		gQueue->Submit({ &*gPrimBuffers[imgIndex] }, {}, { &*gRenderSemaphores[imgIndex] }, nullptr, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

		gSwapchain->Present({ &*gRenderSemaphores[imgIndex] });
	}
	window.Hide();

	gDevice->WaitForIdle();

}
