// RayTracer.cpp : Defines the entry point for the console application.
//

#if _DEBUG
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#else
#pragma comment(linker, "/entry:\"WinMainCRTStartup\"")
#endif

#define WIN32_LEAN_AND_MEAN

#pragma once
#include "stdafx.h"
#include <iostream>

#include "TestAppGame.h"
#include "Window_DX.h"

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3d11_1.h>

#include <stdlib.h> 
#include <time.h>

#include <renderdoc_app.h>

#include "Log.h"
#include "Globals.h"
#include "Config.h"

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	Logger::ReportingLevel() = Severity::VERBOSE;

	// READ IN CONFIG!
	Config appSettings;
	appSettings.ReadInFromFile("../Resources/Settings/AppSettings.cfg");
	int width = appSettings.GetValueAsInt("SCREEN_WIDTH");
	int height = appSettings.GetValueAsInt("SCREEN_HEIGHT");
	bool fullscreen = appSettings.GetValueAsBool("FULL_SCREEN");
	GlobalSettings::Settings().SetWidth(width != 0 ? width : 1280);
	GlobalSettings::Settings().SetHeight(height != 0 ? height : 720);
	GlobalSettings::Settings().SetFullscreen(fullscreen);

	GlobalSettings::Settings().SetRenderDoc(false);
	// Load renderdoc
	if (strcmp(lpCmdLine, "-renderdoc") == 0)
	{
		HINSTANCE hinstLib;
		LoadLibrary("renderdoc.dll");
		RENDERDOC_API_1_1_2 *rdoc_api = NULL;

		// At init, on windows
		if (HMODULE mod = GetModuleHandleA("renderdoc.dll"))
		{
			pRENDERDOC_GetAPI RENDERDOC_GetAPI = (pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
			int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void **)&rdoc_api);
			assert(ret == 1);
			LOG_DEBUG << "Loaded renderdoc";
		}
		else
		{
			LOG_ERROR << "Failed to load renderdoc.dll";
		}
	}

	LOG_INFO << "Starting the application...";

	// Create the Game object
	TestAppGame game;

	// Create a Window object
	Window_DX application(&game, SCREEN_WIDTH, SCREEN_HEIGHT, hInstance, nCmdShow, "Game");

	srand(static_cast<unsigned int>(time(NULL)));

	application.Initialise();
}