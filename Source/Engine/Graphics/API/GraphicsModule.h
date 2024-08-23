#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Application/ApplicationModule.h>

namespace MAGE
{
	class GraphicsAPI;
	class GraphicsModule final : public ApplicationModule
	{
	public:
		GraphicsModule();
		~GraphicsModule() override = default;

		void Start() noexcept override;
		void Update() noexcept override;

		void Stop() noexcept override;

	protected:
		b8 OnInitialize() noexcept override;

		void OnPreUpdate() noexcept override;

		void OnPostUpdate() noexcept override;

	private:
		GraphicsAPI* mGraphicsAPI;
	};
}
