#pragma once
#include "Texture.h"
#include "Globals.h"
#include <crtdbg.h> 

Texture::Texture() :
	miWidth(1024),
	miHeight(1024),
	mFormat(DXGI_FORMAT_R8G8B8A8_UNORM),
	miBindFlags(0),
	miCPUAccessFlags(0),
	meUsage(D3D11_USAGE_DYNAMIC)
{
}

Texture::~Texture()
{
	_ASSERT(mpTexture == NULL);
	_ASSERT(mpTextureSRV == NULL);
}

bool Texture::Initialise(DirectXDevice* device)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = miWidth;
	textureDesc.Height = miHeight;
	textureDesc.Format = mFormat;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = miBindFlags;
	textureDesc.CPUAccessFlags = miCPUAccessFlags;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = meUsage;

	// Create Texture
	HRESULT result = device->GetDevice()->CreateTexture2D(&textureDesc, NULL, &mpTexture);
	_ASSERT(result == S_OK);
	// Create shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = mFormat;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	result = device->GetDevice()->CreateShaderResourceView(mpTexture, &shaderResourceViewDesc, &mpTextureSRV);
	_ASSERT(result == S_OK);

	return (result == S_OK);
}

bool Texture::Release()
{
	ULONG refCount = 0;
	bool success = true;

	// Release the texture
	if (mpTexture)
	{
		refCount = mpTexture->Release();
		_ASSERT(refCount == 0);
		success = (refCount == 0);
		mpTexture = nullptr;
	}

	// Release the shader resource view
	if (mpTextureSRV)
	{
		refCount = mpTextureSRV->Release();
		_ASSERT(refCount == 0);
		success = (refCount == 0);
		mpTextureSRV = nullptr;
	}

	return success;
}

/// Copies texture data from the passed in array "data" into the texture.
/// param device Pointer to the directx device
/// param data Pointer to the texture data you want to copy into the texture
/// param rowPitch The row size of the data you want to copy in Bytes
/// returns true if succeeds, false if failed.
bool Texture::CopyDataIntoTexture(DirectXDevice* device, BYTE * data, const int rowPitch)
{
	// Map the texture.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	HRESULT result = device->GetContext()->Map(mpTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	_ASSERT(result == S_OK);

	if (mappedResource.pData != NULL)
	{
		// Pointer to the data to copy.
		BYTE* pixels = data;
		// Pointer to the texture data.
		BYTE* mappedData = reinterpret_cast<BYTE*>(mappedResource.pData);

		// Copy in the rows of data
		for (int i = 0; i < miHeight; i++)
		{
			memcpy(mappedData, pixels, rowPitch);
			mappedData += mappedResource.RowPitch;
			pixels += rowPitch;
		}

		// Unmap the texture
		device->GetContext()->Unmap(mpTexture, 0);

		return true;
	}
	// Failed to map the texture, or problem accessing the textures data.
	return false;
}