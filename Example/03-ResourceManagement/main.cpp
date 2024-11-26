#include <Engine/Configuration/ConfigCreator.h>
#include <Engine/RenderContext/RenderContext.h>

#include <Engine/Resource/Factory/ResourceFactory.h>

using namespace MAGE;

int main(int argC, char** argV)
{
	Config::InitializeConfig(argC, argV);

	Guid guid = Guid::GenerateID();
	Resource* test = ResourceFactory::Get().GetResourceObject(guid);
}
