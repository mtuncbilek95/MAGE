#include "EntityWindow.h"

#include <imgui.h>

namespace MAGE
{
	EntityWindow::EntityWindow()
	{
	}

	EntityWindow::~EntityWindow()
	{
	}

	void EntityWindow::Update()
	{
		if (!m_dirty)
		{
			ImGui::SetWindowSize("EntityList", ImVec2(200, 200));
			m_dirty = true;
		}
	}

	void EntityWindow::Render()
	{
		ImGui::Begin("EntityList");
		ImGui::Text("Hello, World!");
		ImGui::End();
	}
}