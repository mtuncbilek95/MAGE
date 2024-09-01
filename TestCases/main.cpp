#include "TestApp.h"

using namespace MAGE;

int main(i32 argC, char** argV)
{

	PlatformAPI::GetAPI()->InitializeArguments(argC, argV);

	SharedPtr<TestApp> app = MakeShared<TestApp>();
	app->Run();
}