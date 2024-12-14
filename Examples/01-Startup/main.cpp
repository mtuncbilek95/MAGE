#include <Engine/Renderer/DXRenderer.h>

using namespace Mage;

int main()
{
	DXRenderer::Get()->Start();

	Context::GetDefaultWindow()->Show();
	while (!Context::GetDefaultWindow()->IsClosed())
	{
		Context::GetDefaultWindow()->PollEvents();
		Context::GetDefaultSwapchain()->Present(true);
	}
	Context::GetDefaultWindow()->Hide();
}