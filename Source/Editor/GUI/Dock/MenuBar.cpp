#include "MenuBar.h"

#include <imgui.h>

#include "Editor/Helpers/ImGuiPanelRegistry.h"
#include "Editor/Helpers/ImGuiListener.h"

#include <Engine/Window/WindowManager.h>

namespace MAGE
{
	auto& winMan = Manager::Window::Get();

	MenuBar::MenuBar()
	{
		winMan.GetWindow().WindowPosCallback([this](Math::Vec2i pos) {
			HandlePosition(pos);
			});
	}

	MenuBar::~MenuBar()
	{
	}

	void MenuBar::Update()
	{
	}

	void MenuBar::Render()
	{
		auto& window = Manager::Window::Get().GetWindow();

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene"))
				{
				}
				if (ImGui::MenuItem("Open Scene"))
				{
				}
				if (ImGui::MenuItem("Save Scene"))
				{
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Exit"))
				{
					Manager::Window::Get().GetWindow().Quit();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::EndMenu();
			}

			if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				m_isDrag = true;
				glfwGetCursorPos(winMan.GetWindow().GetGLFWWindow(), &m_clickOffset.x, &m_clickOffset.y);
				glfwGetWindowPos(winMan.GetWindow().GetGLFWWindow(), &m_winPos.x, &m_winPos.y);
			}

			ImGui::EndMenuBar();
		}

		if (m_isDrag)
		{
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			{
				m_isDrag = false;
			}
			else
			{
				Math::Vec2d mousePos;
				glfwGetCursorPos(winMan.GetWindow().GetGLFWWindow(), &mousePos.x, &mousePos.y);

				Math::Vec2i newPos = { m_winPos.x + std::round(mousePos.x - m_clickOffset.x), m_winPos.y + std::round(mousePos.y - m_clickOffset.y) };

				glfwSetWindowPos(winMan.GetWindow().GetGLFWWindow(), newPos.x, newPos.y);
			}
		}
	}

	void MenuBar::HandlePosition(Math::Vec2i pos)
	{
		if (m_isDrag)
			m_winPos = pos;
	}
}