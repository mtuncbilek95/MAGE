#include <Engine/Configuration/ConfigCreator.h>
#include <Engine/RenderContext/RenderContext.h>

#include <Engine/Resource/Helpers/ResourceHelper.h>

using namespace MAGE;

int main(int argC, char** argV)
{
	Config::InitializeConfig(argC, argV);

	RenderContext::Get().GenerateContext();

	while (!Context::GetMainWindow()->IsClosed())
	{
		RenderContext::Get().PrepareFrame();
		RenderContext::Get().SubmitFrame();
	}

	Context::GetMainDevice()->WaitForIdle();
}
