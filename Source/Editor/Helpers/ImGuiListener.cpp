#include "ImGuiListener.h"

namespace MAGE
{
	void ImGuiListener::RegisterCallback(const EventCallback& callback)
	{
		m_callbacks.push_back(callback);
	}

	void ImGuiListener::RemoveCallback(const EventCallback& callback)
	{
		m_callbacks.erase(std::remove_if(m_callbacks.begin(), m_callbacks.end(),
			[&callback](const std::function<void()>& l) {
				return callback.target<void()>() == l.target<void()>();
			}),
			m_callbacks.end());
	}

	void ImGuiListener::Dispatch()
	{
		for (auto& callback : m_callbacks)
			callback();
	}
}