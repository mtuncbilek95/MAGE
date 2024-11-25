#include "Resource.h"

#include "Engine/Resource/Helpers/ResourceHelper.h"
#include <nlohmann/json.hpp>
#include <magic_enum.hpp>

namespace MAGE
{
	Resource::Resource(const ResourceProps& desc) : m_resProps(desc)
	{
	}

	void Resource::Serialize(const path& absPath)
	{
		nlohmann::json json = {};
		json["ResourceProperties"]["Guid"] = m_resProps.guid.ToString().c_str();
		json["ResourceProperties"]["SizeInBytes"] = m_resProps.sizeInBytes;
		json["ResourceProperties"]["MetaPath"] = m_resProps.relativePath;
		json["ResourceProperties"]["ResourceName"] = m_resProps.resourceName;
		json["ResourceProperties"]["ResourceType"] = m_resProps.type;
		json["ResourceProperties"]["LastModified"] = m_resProps.lastModified.ToString().c_str();
		
		// Write data from m_props.data into the binary file
	}

	void Resource::Deserialize(const path& relPath)
	{

	}
}
