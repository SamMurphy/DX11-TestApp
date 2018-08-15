#pragma once
#include "Texture.h"
#include "Globals.h"
#include <crtdbg.h> 

Texture::Texture() :
	miWidth(1024),
	miHeight(1024),
	mFormat(DXGI_FORMAT_R8G8B8A8_UNORM),
	miBindFlags(D3D11_BIND_SHADER_RESOURCE),
	miMiscFlags(0),
	miMipLevels(1),
	miCPUAccessFlags(D3D11_CPU_ACCESS_WRITE),
	meUsage(D3D11_USAGE_DYNAMIC),
	mInitialData(false),
	mTexInitData()
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
	if (miMipLevels > 1)
		textureDesc.MipLevels = 0;
	else
		textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = miBindFlags;
	textureDesc.CPUAccessFlags = miCPUAccessFlags;
	textureDesc.MiscFlags = miMiscFlags;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = meUsage;

	// Create Texture
	HRESULT result;
	if (mInitialData)
	{
		if (miMipLevels > 1)
		{
			result = device->GetDevice()->CreateTexture2D(&textureDesc, NULL, &mpTexture);
			// Copy the first mip into the texture the others will be generated.
			device->GetContext()->UpdateSubresource(mpTexture, 0, NULL, mTexInitData.pSysMem, mTexInitData.SysMemPitch, 0);
		}
		else
		{
			result = device->GetDevice()->CreateTexture2D(&textureDesc, &mTexInitData, &mpTexture);
		}
	}
	else
	{
		result = device->GetDevice()->CreateTexture2D(&textureDesc, NULL, &mpTexture);
	}
	_ASSERT(result == S_OK);

	// Create shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = mFormat;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//result = device->GetDevice()->CreateShaderResourceView(mpTexture, &shaderResourceViewDesc, &mpTextureSRV);
	result = device->GetDevice()->CreateShaderResourceView(mpTexture, NULL, &mpTextureSRV);
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

void Texture::SetInitialData(const void* data, unsigned int pitch, unsigned int depth)
{
	mInitialData = true;
	ZeroMemory(&mTexInitData, sizeof(D3D11_SUBRESOURCE_DATA));
	mTexInitData.pSysMem = data;
	mTexInitData.SysMemPitch = pitch;
	mTexInitData.SysMemSlicePitch = depth;

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