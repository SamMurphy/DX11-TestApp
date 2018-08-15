/**
*  @file DirectXDevice.cpp
*  @brief The concrete DirectX implementation of GraphicsDevice
*
*  The concrete DirectX implementation of GraphicsDevice
*
*
*  @author Sam Murphy
*  @bug No known bugs.
*/
#pragma once
#include "DirectXDevice.h"
#include <iostream>
#include "Window_DX.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_dx11.h"

#include "Globals.h"
#include "Log.h"

#pragma comment(lib, "dxgi.lib")

#define SHOW_CURSOR true

static const float clearColour[4] = { 227.0f / 255.0f, 0, 140.0f / 255.0f, 1.0f }; // Pinkish

DirectXDevice::DirectXDevice(Window_DX* win) : _window(win)
{
	_hWnd = win->GetHWND();
}


DirectXDevice::~DirectXDevice()
{
}

/**
*  @brief Sets up the graphics device.
*
*  Creates all the buffers and states, and the device and context.
*
*  @param width The width of the buffers.
*  @param height The height of the buffers.
*/
void DirectXDevice::Initialise(int width, int height)
{
	HRESULT result;
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                   // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
	scd.BufferDesc.Width = width;							// set the back buffer width
	scd.BufferDesc.Height = height;						 // set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
	scd.OutputWindow = _hWnd;                              // the window to be used
	scd.SampleDesc.Count = 1;                              // how many multisamples
	scd.Windowed = true;                           // windowed/full-screen mode
	scd.Flags = 0;
	//scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL feature = D3D_FEATURE_LEVEL_11_0;

	UINT flags = D3D11_CREATE_DEVICE_DEBUG;

	HRESULT hr;
	// create a device, device context and swap chain using the information in the scd struct
	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		flags,
		&feature,
		1,
		D3D11_SDK_VERSION,
		&scd,
		&_swapchain,
		&_device,
		NULL,
		&_context);
	_ASSERT(hr == S_OK);
	
	IDXGIDevice * pDXGIDevice;
	hr = _device->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
	_ASSERT(hr == S_OK);

	IDXGIAdapter * pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);
	_ASSERT(hr == S_OK);

	IDXGIFactory * lpFactory;
	hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&lpFactory);
	_ASSERT(hr == S_OK);

	hr = lpFactory->MakeWindowAssociation(scd.OutputWindow, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);
	_ASSERT(hr == S_OK);

	ConfigureBackBuffer((float)width, (float)height);

	// Setup rasterizer state
	D3D11_RASTERIZER_DESC raster;
	raster.AntialiasedLineEnable = false;
	raster.CullMode = D3D11_CULL_BACK;
	raster.DepthBias = 0;
	raster.DepthBiasClamp = 0.0f;
	raster.DepthClipEnable = true;
	raster.FillMode = D3D11_FILL_SOLID; // D3D11_FILL_WIREFRAME
	raster.FrontCounterClockwise = false;
	raster.MultisampleEnable = false;
	raster.ScissorEnable = false;
	raster.SlopeScaledDepthBias = 0.0f;

	result = _device->CreateRasterizerState(&raster, &_rasterStateSolid);
	if (FAILED(result))
	{
		std::cout << "Failed to create rasterizer state" << std::endl;
	}
	_context->RSSetState(_rasterStateSolid);

	D3D11_RASTERIZER_DESC raster2;
	raster2.AntialiasedLineEnable = false;
	raster2.CullMode = D3D11_CULL_BACK;
	raster2.DepthBias = 0;
	raster2.DepthBiasClamp = 0.0f;
	raster2.DepthClipEnable = true;
	raster2.FillMode = D3D11_FILL_WIREFRAME;
	raster2.FrontCounterClockwise = false;
	raster2.MultisampleEnable = false;
	raster2.ScissorEnable = false;
	raster2.SlopeScaledDepthBias = 0.0f;

	result = _device->CreateRasterizerState(&raster2, &_rasterStateWireFrame);
	if (FAILED(result))
	{
		std::cout << "Failed to create rasterizer state" << std::endl;
	}

	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	_context->RSSetViewports(1, &viewport);

	// Clear the blend state description.
	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	//blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	result = _device->CreateBlendState(&blendStateDescription, &_alphaBlendingState);
	if (FAILED(result))
	{
		std::cout << "Failed to create blend state" << std::endl;
	}
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	_context->OMSetBlendState(_alphaBlendingState, blendFactor, 0xffffffff);


	// Clear the blend state description.
	D3D11_BLEND_DESC blendStateDescriptionAlphaDisabled;
	ZeroMemory(&blendStateDescriptionAlphaDisabled, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescriptionAlphaDisabled.RenderTarget[0].BlendEnable = FALSE;
	//blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescriptionAlphaDisabled.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescriptionAlphaDisabled.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescriptionAlphaDisabled.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescriptionAlphaDisabled.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescriptionAlphaDisabled.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescriptionAlphaDisabled.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescriptionAlphaDisabled.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	result = _device->CreateBlendState(&blendStateDescriptionAlphaDisabled, &_alphaBlendingDisabledState);
	if (FAILED(result))
	{
		std::cout << "Failed to create blend state" << std::endl;
	}

	ShowCursor(SHOW_CURSOR);
}

/**
*  @brief Clears the screen.
*
*  Clears the render targerts, and depth buffer.
*/
void DirectXDevice::ClearScreen()
{
	_context->ClearRenderTargetView(_backbuffer, clearColour);
	_context->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

/**
*  @brief Swaps the buffers
*  calls Present on the swap chain.
*/
void DirectXDevice::SwapBuffers()
{
#if defined D_USE_IMGUI
	_context->OMSetRenderTargets(1, &_backbuffer, _depthStencilView);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
	_swapchain->Present(0, 0);
}

/**
*  @brief Safely shuts down all the resources the GraphicsDevice uses.
*/
void DirectXDevice::Shutdown()
{
	// switch to windowed mode
	_swapchain->SetFullscreenState(FALSE, NULL);

	// close and release all existing COM objects
	_swapchain->Release();
	_backbuffer->Release();
	_device->Release();
	_context->Release();
	_depthStencilView->Release();
	_depthStencilState->Release();
	_depthDisabledStencilState->Release();
	_depthStencilBuffer->Release();
	_rasterStateSolid->Release();
	_rasterStateWireFrame->Release();
	_alphaBlendingState->Release();
}

/**
*  @brief Enables/Disables wire frame mode
*
*  Swaps the raster state to the one with / without wireframe rendering enabled.  
*
*  @param enable If true, enables wire frame mode. If false, disables wire frame mode.
*/
void DirectXDevice::EnableWireFrame(bool enable)
{
	if (enable)
	{
		_context->RSSetState(_rasterStateWireFrame);
	}
	else
	{
		_context->RSSetState(_rasterStateSolid);
	}
}

/**
*  @brief Enables/Disables depth buffering.
*  Used for 2D rendering.
*
*  @param enable if true, enables depth buffering.
*/
void DirectXDevice::EnableDepthBuffering(bool enable)
{
	if (enable)
	{
		// turn on
		_context->OMSetDepthStencilState(_depthStencilState, 1);
	}
	else
	{
		// turn off
		_context->OMSetDepthStencilState(_depthDisabledStencilState, 1);
	}
}

void DirectXDevice::EnableAlphaBlending(bool enable)
{
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	if (enable)
		_context->OMSetBlendState(_alphaBlendingState, blendFactor, 0xffffffff);
	else
		_context->OMSetBlendState(_alphaBlendingDisabledState, blendFactor, 0xffffffff);
}

/**
*  @brief Enables/Disables wether the cursor renders in the window.
*
*  @param showCursor shows cursor if true, hides it if false.
*/
void DirectXDevice::DisplayCursor(bool showCursor)
{
	ShowCursor(showCursor);
}

void DirectXDevice::ConfigureBackBuffer(float width, float height)
{
	// get the address of the back buffer
	ID3D11Texture2D *p_backbuffer;
	_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&p_backbuffer);

	// use the back buffer address to create the render target
	HRESULT result = 0;
	result = _device->CreateRenderTargetView(p_backbuffer, NULL, &_backbuffer);
	if (FAILED(result))
	{
		std::cout << "Failed to create backbuffer" << std::endl;
	}
	p_backbuffer->Release();
	p_backbuffer = 0;

	// Initialize the description of the depth buffer.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_DESC depthDisableStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = static_cast<UINT>(width);
	depthBufferDesc.Height = static_cast<UINT>(height);
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = _device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer);
	if (FAILED(result))
	{
		std::cout << "Failed to create depth buffer" << std::endl;
	}
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
	if (FAILED(result))
	{
		std::cout << "Failed to create depth stenicl state" << std::endl;
	}
	_context->OMSetDepthStencilState(_depthStencilState, 1);

	// DEPTH DISABLED STATE
	ZeroMemory(&depthDisableStencilDesc, sizeof(depthDisableStencilDesc));

	depthDisableStencilDesc.DepthEnable = false;
	depthDisableStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthDisableStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	depthDisableStencilDesc.StencilEnable = false;
	depthDisableStencilDesc.StencilReadMask = 0xFF;
	depthDisableStencilDesc.StencilWriteMask = 0xFF;

	depthDisableStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisableStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisableStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisableStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthDisableStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisableStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisableStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisableStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = _device->CreateDepthStencilState(&depthDisableStencilDesc, &_depthDisabledStencilState);
	if (FAILED(result))
	{
		std::cout << "Failed to create depth disabled stenicl state" << std::endl;
	}

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = depthBufferDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = _device->CreateDepthStencilView(_depthStencilBuffer, NULL, &_depthStencilView);
	if (FAILED(result))
	{
		std::cout << "Failed to create depth stencil view" << std::endl;
	}
	_context->OMSetRenderTargets(1, &_backbuffer, _depthStencilView);

	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	_context->RSSetViewports(1, &viewport);
}

void DirectXDevice::CleanUpBackBuffer()
{
	if (_backbuffer) 
	{ 
		_backbuffer->Release(); 
		_backbuffer = nullptr; 
	}

	if (_depthStencilBuffer)
	{
		_depthStencilBuffer->Release();
		_depthStencilBuffer = nullptr;
	}

	if (_depthStencilState)
	{
		_depthStencilState->Release();
		_depthStencilState = nullptr;
	}

	if (_depthDisabledStencilState)
	{
		_depthDisabledStencilState->Release();
		_depthDisabledStencilState = nullptr;
	}

	if (_depthStencilView)
	{
		_depthStencilView->Release();
		_depthStencilView = nullptr;
	}
}

void DirectXDevice::SetWindowMode(bool fullscreen, bool borderless, int monitor)
{
	HRESULT hr = S_OK;

	IDXGIFactory* lpFactory;
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&lpFactory);
	_ASSERT(hr == S_OK);

	// NOTE: AH: Currently force to the first adapter (adapter 0) and output (output 0)
	IDXGIAdapter* lpAdapter;
	IDXGIOutput * lpOutput;
	DXGI_ADAPTER_DESC lAdapterDesc;
	DXGI_OUTPUT_DESC lOutputDesc;
	UINT lAdapterId = 0;

	// Get the primary adapter
	hr = lpFactory->EnumAdapters(lAdapterId, &lpAdapter);
	_ASSERT(hr == S_OK);

	hr = lpAdapter->GetDesc(&lAdapterDesc);
	_ASSERT(hr == S_OK);

	hr = lpAdapter->EnumOutputs(monitor, &lpOutput);
	// Couldn't find requested output, try and use default one
	if (hr == DXGI_ERROR_NOT_FOUND)
	{
		_ASSERT(hr == S_OK); // ASSERTING HERE FOR DEBUG PURPOSES
		hr = lpAdapter->EnumOutputs(0, &lpOutput);
		_ASSERT(hr == S_OK);
	}

	if ( borderless )
	{
		if ( GetWindowLongPtr( _hWnd, GWL_STYLE ) & WS_OVERLAPPEDWINDOW )
		{
			hr = lpOutput->GetDesc(&lOutputDesc);
			_ASSERT(hr == S_OK);

			// Get Monitor Information
			MONITORINFO moninfo;
			moninfo.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo(lOutputDesc.Monitor, &moninfo);

			uint32_t height = moninfo.rcWork.bottom - moninfo.rcWork.top;
			uint32_t width = moninfo.rcWork.right - moninfo.rcWork.left;
			

			LOG_DEBUG << "Window is overlapped";
			int w = GetSystemMetrics( SM_CXSCREEN );
			int h = GetSystemMetrics( SM_CYSCREEN );
			SetWindowLongPtr( _hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP );
			SetWindowPos( _hWnd, HWND_TOP, moninfo.rcWork.left, moninfo.rcWork.top, width, height, SWP_FRAMECHANGED );

			ImGui_ImplDX11_InvalidateDeviceObjects();
			CleanUpBackBuffer();
			HRESULT result = _swapchain->ResizeBuffers(
				0,											// Buffer Count, 0 preserves the existing number of buffers in the swap chain
				SCREEN_WIDTH,										// Width - 0 will make DXGI pick the client area
				SCREEN_HEIGHT,										// Height - 0 will make DXGI pick the client area
				DXGI_FORMAT_UNKNOWN,						// Format of the back buffer, UNKNOWN preserves the existing format
				0 /*DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH*/ );	// Swap Chain flags
			_ASSERT( result == S_OK );
			ConfigureBackBuffer( (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT );
			ImGui_ImplDX11_CreateDeviceObjects();
		}
	}
	else
	{
		// Set up the new DXGI_MODE_DESC
		DXGI_MODE_DESC lMode;
		ZeroMemory(&lMode, sizeof(lMode));
		lMode.Width = SCREEN_WIDTH;
		lMode.Height = SCREEN_HEIGHT;
		//lMode.RefreshRate = Desc.BufferDesc.RefreshRate; // Zero'd out so that DXGI handles this
		lMode.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		lMode.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		lMode.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// If the current window style is borderless or fullscreen then change it to the standard style.
		if ((GetWindowLongPtr(_hWnd, GWL_STYLE) & WS_POPUP) || fullscreen == 0)
		{
			SetWindowLongPtr(_hWnd, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);

			// Get the output description, so we can determine the screen position
			DXGI_OUTPUT_DESC lOutputDesc;
			hr = lpOutput->GetDesc(&lOutputDesc);
			_ASSERT(hr == S_OK);

			MONITORINFO moninfo;
			moninfo.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo(lOutputDesc.Monitor, &moninfo);
			// Get the position of the monitor
			int32_t luLeft = moninfo.rcWork.left;
			int32_t luTop = moninfo.rcWork.top;

			SetWindowPos(_hWnd, HWND_TOP, luLeft, luTop, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_FRAMECHANGED);

			DWORD dwWindowStyle = (WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN) ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);
			SetWindowLongPtr(_hWnd, GWL_STYLE, dwWindowStyle);
		}
		// Should be safe to call funcs on the IDXGISwapChain on the main thread as we know the Submit Thread is idle right now.
		if (fullscreen == 1)
		{
			SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

			hr = _swapchain->SetFullscreenState(fullscreen, lpOutput);
			_ASSERT(hr == S_OK);
		}
		else if (fullscreen == 0)
		{
			// Second argument must be NULL if luFullscreen is false.
			hr = _swapchain->SetFullscreenState(fullscreen, NULL);
			_ASSERT(hr == S_OK);

			SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
		}
	}
}

void DirectXDevice::SetSize(float width, float height)
{
	HRESULT hr = S_OK;

	DXGI_MODE_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Height = static_cast<UINT>(height);
	desc.Width = static_cast<UINT>(width);
	desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	hr = _swapchain->ResizeTarget(&desc);
	_ASSERT(hr == S_OK);

	ImGui_ImplDX11_InvalidateDeviceObjects();
	CleanUpBackBuffer();
	HRESULT result = _swapchain->ResizeBuffers(
		0,											// Buffer Count, 0 preserves the existing number of buffers in the swap chain
		static_cast<UINT>(width),										// Width - 0 will make DXGI pick the client area
		static_cast<UINT>(height),										// Height - 0 will make DXGI pick the client area
		DXGI_FORMAT_UNKNOWN,						// Format of the back buffer, UNKNOWN preserves the existing format
		0 /*DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH*/ );	// Swap Chain flags
	_ASSERT( result == S_OK );
	ConfigureBackBuffer( width, height );
	ImGui_ImplDX11_CreateDeviceObjects();
}

int DirectXDevice::GetNumberOfMonitors()
{
	int numberOfMonitors = 0;
	HRESULT hr;
	IDXGIFactory* lpFactory;
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&lpFactory);
	_ASSERT(hr == S_OK);

	IDXGIAdapter* lpAdapter;
	IDXGIOutput * lpOutput;
	DXGI_OUTPUT_DESC lOutputDesc;
	UINT lAdapterId = 0;

	// Get the primary adapter
	hr = lpFactory->EnumAdapters(lAdapterId, &lpAdapter);
	_ASSERT(hr == S_OK);
	

	for (UINT i = 0; lpAdapter->EnumOutputs(i, &lpOutput) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		hr = lpOutput->GetDesc(&lOutputDesc);
		_ASSERT(hr == S_OK);
		lpOutput->Release();
		lpOutput = nullptr;
		numberOfMonitors++;
	}

	lpAdapter->Release();
	lpFactory->Release();

	return numberOfMonitors;
}