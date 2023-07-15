#include <Windows.h>

#include "Window.h"
#include "Application.h"

int WINAPI WinMain( HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount )
{
	try // error handling
	{
		return Application{}.Run();
	}
	catch ( const std::exception& e ) { MessageBox( nullptr, e.what(), "STD Exception", MB_OK | MB_ICONERROR ); }
	catch ( ... ) { MessageBox( nullptr, "No info available", "Unknown Error", MB_OK | MB_ICONERROR ); }
	return 0;

}