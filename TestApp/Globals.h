/**
*  @file Globals.h
*  @brief Used store global variables for the application
*
*  Used to store global variables for the application, access through a single ton, or #defines.
*
*  @author Sam Murphy
*  @bug No known bugs.
*/

#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

#include "ImGui\ImGuiAppLog.h"

#define SCREEN_WIDTH \
	GlobalSettings::Settings().screenWidth
#define SCREEN_HEIGHT \
	GlobalSettings::Settings().screenHeight
#define FULL_SCREEN \
	GlobalSettings::Settings().fullscreen;
#define RENDERDOC \
	GlobalSettings::Settings().renderdoc;
#define IMGUIAPPLOG \
	GlobalSettings::Settings().imGuiAppLog;
#if defined D_USE_IMGUI
#define RENDERLOG \
	GlobalSettings::Settings().renderLog;
#endif

/**
*  @brief Singleton class, used to access global variables for the application.
*  
*  Singleton class that provides access to a number of global variables to the whole application.
*  Stores the screen dimensions and wether it should be fullscreen.
*/
class GlobalSettings
{
public:
	/**
	*  Provides access to the singleton instance.
	*/
	static GlobalSettings& Settings()
	{
		static GlobalSettings instance;
		return instance;
	}

	/// The width of the screen.
	int screenWidth;
	/// The height of the screen.
	int screenHeight;
	/// Should the app be fullscreen.
	bool fullscreen;
	/// Is the app using the renderdoc api.
	bool renderdoc;

#if defined D_USE_IMGUI
	bool renderLog;
	ImGuiAppLog imGuiAppLog;
#endif

	/**
	*  @brief Set the width of the screen.
	*  Sets what the width of the screen should be.
	*  @param width The width of the screen
	*/
	void SetWidth(const int width) { screenWidth = width; }
	/**
	*  @brief Set the height of the screen.
	*  Sets what the height of the screen should be.
	*  @param width The height of the screen
	*/
	void SetHeight(const int height) { screenHeight = height; }
	/**
	*  @brief Sets if the app should be fullscreen.
	*  Sets whether or not the app should be fullscreen.
	*  @param full If the app should be fullscreen.
	*/
	void SetFullscreen(const bool full) { fullscreen = full; }
	/**
	*  @brief Sets if the app is using render doc.
	*  This is enabled if renderdoc.dll is successfully loaded.
	*  @param enabled The value renderdoc will be set to.
	*/
	void SetRenderDoc(const bool enabled) { renderdoc = enabled; }

#if defined D_USE_IMGUI
	void SetRenderLog(const bool render) { renderLog = render; }
#endif

private:
	GlobalSettings() {};
	GlobalSettings(const GlobalSettings&);
	GlobalSettings& operator=(const GlobalSettings&);
};

#endif // GLOBAL_H