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
	void SetMiscFlags(UINT flags) { miMiscFlags |= flags; }
	void SetCPUAccessFlags(UINT flags) { miCPUAccessFlags |= flags; }
	void SetUsage(D3D11_USAGE usage) { meUsage = usage; }

	void SetFormat(DXGI_FORMAT format) { mFormat = format; }

	void SetMipLevels(UINT levels) { miMipLevels = levels; }

	bool CopyDataIntoTexture(DirectXDevice* device, BYTE* data, const int rowPitch);

protected:
	int miWidth;
	int miHeight;
	DXGI_FORMAT mFormat;
	UINT miBindFlags;
	UINT miCPUAccessFlags;
	UINT miMiscFlags;
	D3D11_USAGE meUsage;
	int miMipLevels;

	bool mInitialData;
	D3D11_SUBRESOURCE_DATA mTexInitData;

	ID3D11Texture2D* mpTexture;
	ID3D11ShaderResourceView* mpTextureSRV;
};