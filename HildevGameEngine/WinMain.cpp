#include <Windows.h>
#include "App.h"

int WINAPI WinMain( HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount )
{
	return App{}.Run();
}