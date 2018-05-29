// RayTracer.cpp : Defines the entry point for the console application.
//

//#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#pragma comment(linker, "/entry:\"WinMainCRTStartup\"")
#pragma once
#include "stdafx.h"
#include <iostream>

#include "TestAppGame.h"
#include "Window_DX.h"

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include <stdlib.h> 
#include <time.h>

#include "Log.h"
#include "Globals.h"

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	Logger::ReportingLevel() = Severity::VERBOSE;

	GlobalSettings::Settings().SetWidth(900);
	GlobalSettings::Settings().SetHeight(450);
	GlobalSettings::Settings().SetFullscreen(true);

	LOG_INFO << "Starting the application...";

	// Create the Game object
	TestAppGame game;

	// Create a Window object
	Window_DX application(&game, SCREEN_WIDTH, SCREEN_HEIGHT, hInstance, nCmdShow, "Game");

	srand(time(NULL));

	application.Initialise();
}