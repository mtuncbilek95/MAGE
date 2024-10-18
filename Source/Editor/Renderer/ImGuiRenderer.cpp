#include "ImGuiRenderer.h"

#include <vulkan/vulkan.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>

#include <Engine/VulkanGraphics/Descriptor/VulkanDescPool.h>

#include <Engine/Window/WindowManager.h>
#include <Engine/Renderer/RendererContext.h>

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
		auto& renderer = Gfx::RendererContext::Get();
		m_context = ImGui::CreateContext();
		ImGui::SetCurrentContext(m_context);

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
		io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;

		// Win32 should trigger the scale factor.
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);


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

		m_descPool = MakeOwned<VulkanDescPool>(poolProps, renderer.GetDevice());

		auto instance = renderer.GetInstance();
		auto device = renderer.GetDevice();
		auto gQueue = renderer.GetGraphicsQueue();

		ImGui_ImplVulkan_InitInfo initInfo = {};
		initInfo.Instance = instance->GetInstance();
		initInfo.PhysicalDevice = instance->GetAdapter();
		initInfo.Device = device->GetDevice();
		initInfo.QueueFamily = gQueue->GetFamilyIndex();
		initInfo.Queue = gQueue->GetQueue();
		initInfo.DescriptorPool = m_descPool->GetPool();
		initInfo.MinImageCount = 2;
		initInfo.ImageCount = 3;
		initInfo.RenderPass = nullptr;
		initInfo.MinAllocationSize = 1024 * 1024;
		initInfo.UseDynamicRendering = true;

		VkPipelineRenderingCreateInfo renderingInfo = {};
		renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
		renderingInfo.colorAttachmentCount = 1;
		renderingInfo.depthAttachmentFormat = VK_FORMAT_UNDEFINED;
		renderingInfo.pColorAttachmentFormats = format;
		renderingInfo.viewMask = 0;
		renderingInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;

		initInfo.PipelineRenderingCreateInfo = renderingInfo;
		initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

		GLFWwindow* window = Manager::Window::Get().GetWindow().GetGLFWWindow();
		ImGui_ImplGlfw_InitForVulkan(window, true);
		ImGui_ImplVulkan_Init(&initInfo);
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

		auto& renderer = Gfx::RendererContext::Get();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), renderer.GetCmdBuffer()->GetCmdBuffer());
	}

	void ImGuiRenderer::Shutdown()
	{
		panelRegistry.ClearPanels();

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext(m_context);

		m_descPool.reset();
	}

	void ImGuiRenderer::Render()
	{
		listener.Dispatch();
	}
}
