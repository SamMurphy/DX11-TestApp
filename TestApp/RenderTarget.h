#pragma once
#include <D3D11.h>
#include "DirectXDevice.h"

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();

	bool Initialise(DirectXDevice* device);
	bool Release();

	ID3D11Texture2D* GetTexture() { return mpTexture2D; }
	ID3D11RenderTargetView* GetRenderTargetView() { return mpRenderTargetView; }
	ID3D11RenderTargetView** GetAddressOfRenderTargetView() { return &mpRenderTargetView; }
	ID3D11ShaderResourceView* GetShaderResourceView() { return mpShaderResourceView; }
	ID3D11ShaderResourceView** GetAddressOfShaderResourceView() { return &mpShaderResourceView; }


	void SetWidth(UINT width) { miWidth = width; }
	void SetHeight(UINT height) { miHeight = height; }

	void SetDimensionsToFullscreen();

private:
	ID3D11Texture2D * mpTexture2D;
	ID3D11RenderTargetView*	mpRenderTargetView;
	ID3D11ShaderResourceView* mpShaderResourceView;

	UINT miWidth;
	UINT miHeight;
};

