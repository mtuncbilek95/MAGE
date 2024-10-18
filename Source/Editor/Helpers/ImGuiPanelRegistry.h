/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Helpers/Singleton.h>
#include "Editor/Core/ImGuiPanel.h"

namespace MAGE
{
	/**
	 * @class ImGuiPanelRegistry
	 * @brief A registry for MVVM ImGui panels. It will hold every ImGui panel in
	 * the startup. To make them visible or invisible, We will have SetVisible
	 * function. Function will call Listener and add the Update and Render functions
	 * in to the Listener.
	 */
	class ImGuiPanelRegistry final : public Singleton<ImGuiPanelRegistry>
	{
	public:
		ImGuiPanelRegistry() = default;
		~ImGuiPanelRegistry() = default;

		/**
		 * @brief Register a panel to the registry.
		 * @param panel The panel to register.
		 */
		template<typename T, typename...Args, typename = std::enable_if_t<std::is_base_of_v<ImGuiPanel, T>>>
		T* RegisterPanel(Args&&... args)
		{
			Shared<T> panel = MakeShared<T>(std::forward<Args>(args)...);
			m_panels.push_back(panel);
			return panel.get();
		}

		/**
		 * @brief Unregister a panel from the registry.
		 * @param panel The panel to unregister.
		 */
		template<typename T, typename = std::enable_if_t<std::is_base_of_v<ImGuiPanel, T>>>
		void UnregisterPanel(T* panel)
		{
			//m_Panels.erase(std::remove_if(m_Panels.begin(), m_Panels.end(),
			//	[panel](const Owned<ImGuiPanel>& p)
			//	{ 
			//		return p.get() == panel;
			//	}), m_Panels.end());
		}

		void ClearPanels()
		{
			m_panels.clear();
		}

	private:
		Vector<Shared<ImGuiPanel>> m_panels;
	};
}
