/**
*  @file DirectXDevice.h
*  @brief The concrete DirectX implementation of GraphicsDevice
*
*  The concrete DirectX implementation of GraphicsDevice
*
*
*  @author Sam Murphy
*  @bug No known bugs.
*/
#pragma once
#include <D3D11.h>
#include <d3d11_1.h>

// Forward declarations
class Window_DX;

/**
*  @brief The concrete DirectX implementation of GraphicsDevice
*
*  The concrete DirectX implementation of GraphicsDevice
*/
class DirectXDevice
{
public:
	DirectXDevice(Window_DX* win);
	virtual ~DirectXDevice();

	virtual void Initialise(int width, int height);
	virtual void ClearScreen();
	virtual void SwapBuffers();
	virtual void Shutdown();

	ID3D11Device* GetDevice() const { return _device; }
	void SetDevice(ID3D11Device* v) { _device = v; }

	ID3D11DeviceContext* GetContext() const { return _context; }
	void SetContext(ID3D11DeviceContext* v) { _context = v; }

	IDXGISwapChain* GetSwapChain() const { return _swapchain; }

	virtual void EnableWireFrame(bool enable);
	virtual void EnableDepthBuffering(bool enable);
	virtual void EnableAlphaBlending(bool enable);

	void DisplayCursor(bool showCursor);

	void ConfigureBackBuffer(float width, float height);
	void CleanUpBackBuffer();

	void SetWindowMode(bool fullscreen, bool borderless, int monitor);
	void SetSize(float width, float height);

	int GetNumberOfMonitors();

	ID3D11DepthStencilView* GetDepthStencilView() { return _depthStencilView; }
	ID3D11ShaderResourceView** GetAddressOfDepthStencilSRV() { return &_depthStencilBufferSRV; }
	ID3D11RenderTargetView* GetBackBuffer() { return _backbuffer; }
	ID3D11RenderTargetView** GetAddressOfBackBuffer() { return &_backbuffer; }

protected:
	/// The game window.
	Window_DX * _window;
	/// the pointer to the swap chain interface
	IDXGISwapChain*	_swapchain;
	/// the pointer to our Direct3D device interface
	ID3D11Device*	_device;
	/// the pointer to our Direct3D device context
	ID3D11DeviceContext* _context;
	/// the pointer to our back buffer
	ID3D11RenderTargetView* _backbuffer;
	/// The pointer to the depth stencil buffer.
	ID3D11Texture2D* _depthStencilBuffer;
	/// The pointer tho the depth stencil buffers shader resource view.
	ID3D11ShaderResourceView* _depthStencilBufferSRV;
	/// The pointer to the depth stencil state.
	ID3D11DepthStencilState* _depthStencilState;
	/// The pointer to the depth stencil state, where depth buffering is disabled.
	ID3D11DepthStencilState* _depthDisabledStencilState;
	/// The pointer to the depth stencil view.
	ID3D11DepthStencilView* _depthStencilView;
	/// The pointer to the raster state - solid rendering
	ID3D11RasterizerState* _rasterStateSolid;
	/// The pointer to the raster  state for wireframe rendering.
	ID3D11RasterizerState* _rasterStateWireFrame;
	/// The pointer to the alpha blending state.
	ID3D11BlendState* _alphaBlendingState;
	/// The pointer to the alpha blending disabled state.
	ID3D11BlendState* _alphaBlendingDisabledState;
	/// Window handle
	HWND _hWnd;
};

