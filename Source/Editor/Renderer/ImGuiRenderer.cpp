#include "ImGuiRenderer.h"

#include <vulkan/vulkan.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>

namespace MAGE
{
#if defined(DELUSION_WINDOWS)
	const VkFormat format[1] = { VK_FORMAT_R8G8B8A8_UNORM };
#elif defined(DELUSION_LINUX)
	const VkFormat format[1] = { VK_FORMAT_B8G8R8A8_UNORM };
#endif

	ImGuiRenderer::ImGuiRenderer()
	{
	}

	ImGuiRenderer::~ImGuiRenderer()
	{
		Shutdown();
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

		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), nullptr);
	}

	void ImGuiRenderer::Shutdown()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext(m_context);
	}
}
