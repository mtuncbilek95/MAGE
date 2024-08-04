#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/IObject.h>
#include <Engine/Graphics/Instance/GraphicsInstanceDesc.h>

namespace MAGE
{
	class GraphicsInstance : public IObject
	{
	public:
		GraphicsInstance(const GraphicsInstanceDesc& desc) : mAppName(desc.AppName), 
			mAppVersion(desc.AppVersion), mEngineName(desc.EngineName), 
			mEngineVersion(desc.EngineVersion) {}
		virtual ~GraphicsInstance() override = default;

		const String& GetAppName() const { return mAppName; }
		const Vec3f& GetAppVersion() const { return mAppVersion; }
		const String& GetEngineName() const { return mEngineName; }
		const Vec3f& GetEngineVersion() const { return mEngineVersion; }

	private:
		String mAppName;
		Vec3f mAppVersion;

		String mEngineName;
		Vec3f mEngineVersion;
	};
}
