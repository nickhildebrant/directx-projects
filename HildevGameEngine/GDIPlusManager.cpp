#include "GDIPlusManager.h"
#include <algorithm>
#include <gdiplus.h>

#pragma comment(lib,"gdiplus.lib")

namespace Gdiplus
{
	using std::min;
	using std::max;
}

ULONG_PTR GDIPlusManager::token = 0;
int GDIPlusManager::refCount = 0;

GDIPlusManager::GDIPlusManager()
{
	if ( refCount++ == 0 )
	{
		Gdiplus::GdiplusStartupInput input;
		input.GdiplusVersion = 1;
		input.DebugEventCallback = nullptr;
		input.SuppressBackgroundThread = false;
		Gdiplus::GdiplusStartup( &token, &input, nullptr );
	}
}

GDIPlusManager::~GDIPlusManager()
{
	if ( --refCount == 0 )
	{
		Gdiplus::GdiplusShutdown( token );
	}
}