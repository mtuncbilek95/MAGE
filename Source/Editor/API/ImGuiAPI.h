#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/API.h>

#include <Editor/ImguiRenderer/ImGuiRenderer.h>

namespace MAGE
{
	class ImGuiAPI : public API<ImGuiAPI>
	{
		friend class ImGuiRenderModule;

	public:
		ImGuiAPI() = default;
		virtual ~ImGuiAPI() override = default;

		ImGuiRenderer* GetEditorRenderer() { return mEditorRenderer.get(); }

	private:
		WeakPtr<ImGuiRenderer> CreateEditor()
		{
			if (!mEditorRenderer)
				mEditorRenderer = MakeShared<ImGuiRenderer>();

			return mEditorRenderer;
		}

	private:
		SharedPtr<ImGuiRenderer> mEditorRenderer;
	};
}
