#pragma once
#include <D3D11.h>
#include "DirectXDevice.h"

class Texture
{
public:
	Texture();
	virtual ~Texture();

	virtual bool Initialise(DirectXDevice* device);
	virtual bool Release();

	ID3D11Texture2D* GetTexture() { return mpTexture; }
	ID3D11Texture2D** GetAddressOfTexture() { return &mpTexture; }
	ID3D11ShaderResourceView* GetShaderResourceView() { return mpTextureSRV; }
	ID3D11ShaderResourceView** GetAddressOfShaderResourceView() { return &mpTextureSRV; }

	void SetDimensions(UINT width, UINT height) { miWidth = width; miHeight = height; }
	void SetWidth(UINT width) { miWidth = width; }
	void SetHeight(UINT height) { miHeight = height; }

	void SetInitialData(const void* data, unsigned int pitch, unsigned int depth);

	void ResetFlags() { miBindFlags = 0; miCPUAccessFlags = 0; }
	void SetBindFlags(UINT flags) { miBindFlags |= flags; }
	void SetCPUAccessFlags(UINT flags) { miCPUAccessFlags |= flags; }

	void SetFormat(DXGI_FORMAT format) { mFormat = format; }

	bool CopyDataIntoTexture(DirectXDevice* device, BYTE* data, const int rowPitch);

protected:
	int miWidth;
	int miHeight;
	DXGI_FORMAT mFormat;
	UINT miBindFlags;
	UINT miCPUAccessFlags;
	D3D11_USAGE meUsage;

	bool mInitialData;
	D3D11_SUBRESOURCE_DATA mTexInitData;

	ID3D11Texture2D* mpTexture;
	ID3D11ShaderResourceView* mpTextureSRV;
};