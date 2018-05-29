/**
*  @file Window_DX.cpp
*  @brief A DirectX-specific Game Window.
*
*  @author Sam Murphy
*  @bug No known bugs.
*/
#pragma once
#include "Window_DX.h"
#include "DirectXDevice.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_dx11.h"

#include "Log.h"

#include "TestAppGame.h"

Window_DX* Window_DX::TheWindow = 0;

/*----------------------------------------------------------------------------------------------------------------*/
// CONSTRUCTORS
/*----------------------------------------------------------------------------------------------------------------*/

Window_DX::Window_DX(Game* newGame, const int newWidth, const int newHeight, const HINSTANCE hInstance, const int nCmdShow, const LPCTSTR name)
	: game(newGame), width(newWidth), height(newHeight)
{
	LOG_INFO << "Initialising Window: " << newWidth << "x" << newHeight;
	// Reset RNG
	srand(time(NULL));

	TheWindow = this;

	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Window_DX::WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "WindowClass";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, newWidth, newHeight };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL,
		"WindowClass",
		name,
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(GetHWND());
}

/*----------------------------------------------------------------------------------------------------------------*/

Window_DX::~Window_DX()
{
}

/*----------------------------------------------------------------------------------------------------------------*/
// FUNCTIONS
/*----------------------------------------------------------------------------------------------------------------*/

/*
*  Initialise the Game Window
*/
void Window_DX::Initialise()
{
	LOG_INFO << "Initialising Game";
	GetGame()->Initialise(this);

#if defined D_USE_IMGUI
	// Setup ImGui binding
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	ImGui_ImplDX11_Init(hWnd, GetGame()->GetDevice()->GetDevice(), GetGame()->GetDevice()->GetContext());

	// Setup style
	ImGui::StyleColorsLight();
	//ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	ImGuiStyle& style = ImGui::GetStyle();
	const float rounding = 5.0f;
	style.FrameRounding = rounding;
	style.ChildRounding = rounding;
	style.GrabRounding = rounding;
	style.PopupRounding = rounding;
	style.ScrollbarRounding = rounding;
	style.WindowRounding = rounding;
	style.WindowBorderSize = 0.0f;
	style.Colors[2].w = 0.8f;
	ImGui::GetIO().Fonts->AddFontFromFileTTF("../Resources/Roboto-Regular.ttf", 14.0f);
	ImGui::GetIO().Fonts->AddFontFromFileTTF("../Resources/Roboto-Regular.ttf", 16.0f);
	ImGui::GetIO().Fonts->AddFontFromFileTTF("../Resources/Roboto-Regular.ttf", 18.0f);
	ImGui::GetIO().Fonts->AddFontFromFileTTF("../Resources/Roboto-Regular.ttf", 20.0f);
#endif

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (GetGame()->GetQuitFlag() == false)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);	

			if (msg.message == WM_QUIT) // needed to exit by X properly
				break;
		}
		else
		{
#if defined D_USE_IMGUI
			ImGui_ImplDX11_NewFrame();
#endif
			GetGame()->Run();
		}
	}

	LOG_INFO << "Shuting Down Game";
#if defined D_USE_IMGUI
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
#endif
	GetGame()->Shutdown();
}

/*----------------------------------------------------------------------------------------------------------------*/

/*
*  Check for input from mouse and keyboard
*
*  @param hwnd Window Handle
*  @param message Incoming Message
*  @param wParam 1st Message Parameter
*  @param lParam 2nd Message Parameter
*/
#if defined D_USE_IMGUI
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
LRESULT CALLBACK Window_DX::WindowProc(const HWND hwnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
#if defined D_USE_IMGUI
	if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam))
		return true;
#endif

	switch (message)
	{
	case WM_KEYDOWN:
	{
#if defined D_USE_IMGUI
		if (!ImGui::GetIO().WantCaptureKeyboard)
#endif
			TheWindow->GetGame()->OnKeypress(wParam, true);
	}
	break;
	case WM_KEYUP:
	{
#if defined D_USE_IMGUI
		if (!ImGui::GetIO().WantCaptureKeyboard)
#endif
			TheWindow->GetGame()->OnKeypress(wParam, false);
	}
	break;
	case WM_LBUTTONDOWN:
	{
#if defined D_USE_IMGUI
		if (!ImGui::GetIO().WantCaptureMouse)
#endif
		{
			float x = GET_X_LPARAM(lParam);
			float y = GET_Y_LPARAM(lParam);
			TheWindow->GetGame()->OnMouseDown(x, y, 1, true);
		}
	}
	break;
	case WM_LBUTTONUP:
	{
#if defined D_USE_IMGUI
		if (!ImGui::GetIO().WantCaptureMouse)
#endif
		{
			float x = GET_X_LPARAM(lParam);
			float y = GET_Y_LPARAM(lParam);
			TheWindow->GetGame()->OnMouseDown(x, y, 1, false);
		}
	}
	break;
	case WM_RBUTTONDOWN:
	{
#if defined D_USE_IMGUI
		if (!ImGui::GetIO().WantCaptureMouse)
#endif
		{
			float x = GET_X_LPARAM(lParam);
			float y = GET_Y_LPARAM(lParam);
			TheWindow->GetGame()->OnMouseDown(x, y, 2, true);
		}
	}
	break;
	case WM_RBUTTONUP:
	{
#if defined D_USE_IMGUI
		if (!ImGui::GetIO().WantCaptureMouse)
#endif
		{
			float x = GET_X_LPARAM(lParam);
			float y = GET_Y_LPARAM(lParam);
			TheWindow->GetGame()->OnMouseDown(x, y, 2, false);
		}
	}
	break;
	case WM_MBUTTONDOWN:
	{
#if defined D_USE_IMGUI
		if (!ImGui::GetIO().WantCaptureMouse)
#endif
		{
			float x = GET_X_LPARAM(lParam);
			float y = GET_Y_LPARAM(lParam);
			TheWindow->GetGame()->OnMouseDown(x, y, 16, true);
		}
	}
	break;
	case WM_MBUTTONUP:
	{
#if defined D_USE_IMGUI
		if (!ImGui::GetIO().WantCaptureMouse)
#endif
		{
			float x = GET_X_LPARAM(lParam);
			float y = GET_Y_LPARAM(lParam);
			TheWindow->GetGame()->OnMouseDown(x, y, 16, false);
		}
	}
	break;
	case WM_XBUTTONDOWN:
	{
#if defined D_USE_IMGUI
		if (!ImGui::GetIO().WantCaptureMouse)
#endif
		{
			float x = GET_X_LPARAM(lParam);
			float y = GET_Y_LPARAM(lParam);
			TheWindow->GetGame()->OnMouseDown(x, y, wParam, true);
		}
	}
	break;
	case WM_XBUTTONUP:
	{
#if defined D_USE_IMGUI
		if (!ImGui::GetIO().WantCaptureMouse)
#endif
		{
			float x = GET_X_LPARAM(lParam);
			float y = GET_Y_LPARAM(lParam);
			TheWindow->GetGame()->OnMouseDown(x, y, wParam, false);
		}
	}
	break;
	case WM_MOUSEWHEEL:
	{
#if defined D_USE_IMGUI
		if (!ImGui::GetIO().WantCaptureMouse)
#endif
		{
			float x = GET_X_LPARAM(lParam);
			float y = GET_Y_LPARAM(lParam);
			TheWindow->GetGame()->OnMouseDown(x, y, wParam, true);
		}
	}
	break;
	case WM_MOUSEMOVE:
	{
#if defined D_USE_IMGUI
		if (!ImGui::GetIO().WantCaptureMouse)
#endif
		{
			float x = GET_X_LPARAM(lParam);
			float y = GET_Y_LPARAM(lParam);
			TheWindow->GetGame()->OnMouseMove(x, y);
		}
	}
	break;
	case WM_SIZE:
	{
		if (TheWindow->GetDevice() != NULL && wParam != SIZE_MINIMIZED)
		{
			//ImGui_ImplDX11_InvalidateDeviceObjects();
			//TheWindow->GetDevice()->CleanUpBackBuffer();
			//HRESULT result = TheWindow->GetDevice()->GetSwapChain()->ResizeBuffers(
			//	0,											// Buffer Count, 0 preserves the existing number of buffers in the swap chain
			//	( UINT )LOWORD( lParam ),					// Width - 0 will make DXGI pick the client area
			//	( UINT )HIWORD( lParam ),					// Height - 0 will make DXGI pick the client area
			//	DXGI_FORMAT_UNKNOWN,						// Format of the back buffer, UNKNOWN preserves the existing format
			//	DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH );	// Swap Chain flags
			//_ASSERT(result == S_OK);
			//TheWindow->GetDevice()->ConfigureBackBuffer((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
			//ImGui_ImplDX11_CreateDeviceObjects();
		}
	}
	break;
	case WM_SYSKEYDOWN:
	{
		if (wParam == VK_RETURN)
		{
			if ((HIWORD(lParam) & KF_ALTDOWN))
			{
				// Alt-Enter is pressed so toggle-fullscreen
				LOG_DEBUG << "Alt-Enter is pressed";

				TestAppGame* myGame = static_cast<TestAppGame*>(TheWindow->GetGame());
				bool lbFullscreen = !myGame->GetFullscreen();
				myGame->SetFullscreen(lbFullscreen);
				TheWindow->GetDevice()->SetWindowMode(lbFullscreen, false, 0);
			}
		}
	}
	break;
	case WM_ACTIVATE:
	{
	case WM_ACTIVATEAPP:
	{
		// Handles alt-tab
		// Don't do anything if the window mode hasn't been set for the first time.
		TestAppGame* lpGame = static_cast<TestAppGame*>(TheWindow->GetGame());
		bool lbFullscreen = lpGame->GetFullscreen();
		if (lbFullscreen)
		{
			// If the app has gained focus again, switch back to full screen.
			if (wParam != FALSE)
			{
				IDXGISwapChain* lpSwapChain = TheWindow->GetDevice()->GetSwapChain();
				if (lpSwapChain)
				{
					// Minimised the app when we lost focus, so show the window again
					ShowWindow(hwnd, SW_SHOWNA);
					// Go fullscreen mode
					lpGame->SetFullscreen(true);
				}
			}
			// The app has lost focus, so minimise it.
			else
			{
				IDXGISwapChain* lpSwapChain = TheWindow->GetDevice()->GetSwapChain();
				if (lpSwapChain)
				{
					HRESULT hr = lpSwapChain->SetFullscreenState(false, NULL);
					_ASSERT(hr == S_OK);
					ShowWindow(hwnd, SW_SHOWMINNOACTIVE);
				}
			}
		}
	}
	break;
	}
	break;
	case WM_QUIT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0); // needed to exit by X properly
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

/*----------------------------------------------------------------------------------------------------------------*/