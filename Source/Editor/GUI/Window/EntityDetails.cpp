#include "EntityDetails.h"

#include <imgui.h>

namespace MAGE
{
	EntityDetails::EntityDetails()
	{
	}

	EntityDetails::~EntityDetails()
	{
	}

	void EntityDetails::Update()
	{
		if (!m_dirty)
		{
			ImGui::SetWindowSize("EntityDetails", ImVec2(200, 200));
			m_dirty = true;
		}
	}

	void EntityDetails::Render()
	{
		ImGui::Begin("EntityDetails");
		ImGui::Text("Hello, World!");
		ImGui::End();
	}
}