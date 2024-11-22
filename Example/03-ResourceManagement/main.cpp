#include <Engine/RenderContext/RenderContext.h>

using namespace MAGE;

int main()
{
	RenderContext::Get().GenerateContext();

	while (!Context::GetMainWindow()->IsClosed())
	{
		RenderContext::Get().PrepareFrame();
		RenderContext::Get().SubmitFrame();
	}

	Context::GetMainDevice()->WaitForIdle();
}
