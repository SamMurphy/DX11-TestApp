#pragma once
#include "Texture.h"
#include <D3D11.h>
#include "DirectXDevice.h"

class RenderTarget : public Texture
{
public:
	RenderTarget();
	~RenderTarget();

	bool Initialise(DirectXDevice* device);
	bool Release();

	ID3D11RenderTargetView* GetRenderTargetView() { return mpRenderTargetView; }
	ID3D11RenderTargetView** GetAddressOfRenderTargetView() { return &mpRenderTargetView; }

	void SetDimensionsToFullscreen();

private:
	ID3D11RenderTargetView*	mpRenderTargetView;
};