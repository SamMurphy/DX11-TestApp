/**
*  @file Window_DX.h
*  @brief A DirectX-specific Game Window.
*
*  @author Sam Murphy
*  @bug No known bugs.
*/
#pragma once
#include "stdafx.h"
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <time.h>
#include "Game.h"

// Forward Declaration
class Game;

/*
*  A DirectX-specific Game Window.
*/
class Window_DX
{
// Data
private:

	/// The width of the window.
	int width;
	/// The height of the window.
	int height;
	HWND hWnd;
	/// A pointer to the games.
	Game* game;

// Constructors
public:
	Window_DX(Game* newGame, const int newWidth, const int newHeight, const HINSTANCE hInstance, const int nCmdShow, const LPCTSTR name);
	~Window_DX();

// Functions
	static LRESULT CALLBACK WindowProc(const HWND hwnd, const UINT message, const WPARAM wParam, const LPARAM lParam);
	void Initialise();

	const int GetWidth()	const { return width; }
	const int GetHeight()	const { return height; }
	const HWND GetHWND()	const { return hWnd; }
	Game* GetGame() { return game; }
	DirectXDevice* GetDevice() { if (game) return game->GetDevice(); else return nullptr; }

	const void SetHWND(HWND hwnd) { hWnd = hwnd; }

	/// Pointer to the instance of the window.
	static Window_DX* TheWindow;
};

