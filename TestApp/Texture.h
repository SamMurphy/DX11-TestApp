#pragma once
#include <D3D11.h>
#include "DirectXDevice.h"

class Texture
{
public:
	Texture();
	~Texture();

	bool Initialise(DirectXDevice* device);
	bool Release();

	ID3D11Texture2D* GetTexture() { return mpTexture; }
	ID3D11ShaderResourceView* GetShaderResourceView() { return mpTextureSRV; }
	ID3D11ShaderResourceView** GetAddressOfShaderResourceView() { return &mpTextureSRV; }

	void SetWidth(UINT width) { miWidth = width; }
	void SetHeight(UINT height) { miHeight = height; }

protected:
	int miWidth;
	int miHeight;
	DXGI_FORMAT mFormat;

	ID3D11Texture2D* mpTexture;
	ID3D11ShaderResourceView* mpTextureSRV;
};

