#include "ImGuiRenderer.h"

#include <vulkan/vulkan.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>

#include <Engine/VulkanRHI/Descriptor/VDescPool.h>
#include <Engine/VulkanRHI/RenderPass/VRenderPass.h>
#include <Engine/VulkanRHI/Command/VCmdPool.h>
#include <Engine/VulkanRHI/Command/VCmdBuffer.h>

#include <Engine/Window/WindowManager.h>
#include <Engine/Rendering/RenderContext.h>

#include "Editor/Helpers/ImGuiListener.h"
#include "Editor/Helpers/ImGuiPanelRegistry.h"
#include "Editor/GUI/Dock/ImGuiDock.h"

namespace MAGE
{
#if defined(DELUSION_WINDOWS)
	const VkFormat format[1] = { VK_FORMAT_R8G8B8A8_UNORM };
#elif defined(DELUSION_LINUX)
	const VkFormat format[1] = { VK_FORMAT_B8G8R8A8_UNORM };
#endif

	ImGuiPanelRegistry& panelRegistry = ImGuiPanelRegistry::Get();
	ImGuiListener& listener = ImGuiListener::Get();

	Gfx::Context& gfxContext = Gfx::Context::Get();

	ImGuiRenderer::ImGuiRenderer()
	{
		m_dock = panelRegistry.RegisterPanel<ImGuiDock>();
	}

	ImGuiRenderer::~ImGuiRenderer()
	{
		m_dock->SetVisibility(false);
	}

	void ImGuiRenderer::Init()
	{
		m_context = ImGui::CreateContext();
		ImGui::SetCurrentContext(m_context);

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
		io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;

		// Win32 should trigger the scale factor.
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

		io.Fonts->AddFontFromFileTTF(R"(D:\Projects\MAGE\Resources\Fonts\Poppins\Regular.ttf)", 18.f);


		DescPoolProps poolProps = {};
		poolProps.maxSets = 1000;
		poolProps.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolProps.poolSizes =
		{
			{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
			{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
			{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
			{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
			{VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
			{VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
			{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
			{VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}
		};

		m_descPool = MakeOwned<VDescPool>(poolProps, gfxContext.GetDevice());

		auto instance = gfxContext.GetInstance();
		auto device = gfxContext.GetDevice();
		auto gQueue = gfxContext.GetGraphicsQueue();

		ImGui_ImplVulkan_InitInfo initInfo = {};
		initInfo.Instance = instance->GetInstance();
		initInfo.PhysicalDevice = instance->GetAdapter();
		initInfo.Device = device->GetDevice();
		initInfo.QueueFamily = gQueue->GetFamilyIndex();
		initInfo.Queue = gQueue->GetQueue();
		initInfo.DescriptorPool = m_descPool->GetPool();
		initInfo.MinImageCount = 2;
		initInfo.ImageCount = 3;
		initInfo.RenderPass = gfxContext.GetRenderPass()->GetRenderPass();
		initInfo.MinAllocationSize = 1024 * 1024;
		initInfo.UseDynamicRendering = false;

		GLFWwindow* window = Manager::Window::Get().GetWindow().GetGLFWWindow();
		ImGui_ImplGlfw_InitForVulkan(window, true);
		ImGui_ImplVulkan_Init(&initInfo);

		CmdPoolProps poolProp =
		{
			.queue = gfxContext.GetGraphicsQueue(),
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
		};
		m_pool = MakeOwned<VCmdPool>(poolProp, gfxContext.GetDevice());
		m_buffer = m_pool->CreateCmdBuffer(VK_COMMAND_BUFFER_LEVEL_SECONDARY);
	}

	void ImGuiRenderer::BeginFrame()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiRenderer::EndFrame()
	{
		ImGui::Render();

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		m_buffer->BeginRecording(gfxContext.GetRenderPass(), gfxContext.GetFramebuffer());
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_buffer->GetCmdBuffer());
		m_buffer->EndRecording();

		gfxContext.Execute(&*m_buffer);
	}

	void ImGuiRenderer::Shutdown()
	{
		gfxContext.GetDevice()->WaitForIdle();

		panelRegistry.ClearPanels();

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext(m_context);

		m_descPool->Destroy();
		m_buffer->Destroy();
		m_pool->Destroy();
	}

	void ImGuiRenderer::Render()
	{
		listener.Dispatch();
	}
}
