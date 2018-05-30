#include "Texture.h"
#include "Globals.h"
#include <crtdbg.h> 



Texture::Texture()
{
}


Texture::~Texture()
{
}

bool Texture::Initialise(DirectXDevice* device)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = miWidth;
	textureDesc.Height = miHeight;
	textureDesc.Format = mFormat;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DYNAMIC;

	HRESULT result = device->GetDevice()->CreateTexture2D(&textureDesc, NULL, &mpTexture);
	_ASSERT(result == S_OK);
	// Create shader resource view
	result = device->GetDevice()->CreateShaderResourceView(mpTexture, NULL, &mpTextureSRV);
	_ASSERT(result == S_OK);

	return (result == S_OK);
}

bool Texture::Release()
{
	ULONG refCount = 0;
	bool success = true;

	if (mpTexture)
	{
		refCount = mpTexture->Release();
		_ASSERT(refCount == 0);
		success = (refCount == 0);
		mpTexture = nullptr;
	}

	if (mpTextureSRV)
	{
		refCount = mpTextureSRV->Release();
		_ASSERT(refCount == 0);
		success = (refCount == 0);
		mpTextureSRV = nullptr;
	}

	return success;
}
