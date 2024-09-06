#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Object/IObject.h"
#include "Engine/Graphics/Instance/GraphicsInstanceDesc.h"

namespace MAGE
{
	/**
	 * @class GraphicsInstance
	 * @brief Interface for the base generator of the graphics systems.
	 */
	class GraphicsInstance : public IObject
	{
	public:
		GraphicsInstance(const GraphicsInstanceDesc& desc) : mAppName(desc.AppName), 
			mAppVersion(desc.AppVersion), mEngineName(desc.EngineName), 
			mEngineVersion(desc.EngineVersion) {}
		virtual ~GraphicsInstance() override = default;

		const String& GetAppName() const { return mAppName; }
		const Math::Vec3c& GetAppVersion() const { return mAppVersion; }
		const String& GetEngineName() const { return mEngineName; }
		const Math::Vec3c& GetEngineVersion() const { return mEngineVersion; }

	private:
		String mAppName;
		Math::Vec3c mAppVersion;

		String mEngineName;
		Math::Vec3c mEngineVersion;
	};
}
