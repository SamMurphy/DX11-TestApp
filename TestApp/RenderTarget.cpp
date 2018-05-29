#include "RenderTarget.h"
#include "Globals.h"
#include <crtdbg.h>  

RenderTarget::RenderTarget() : mpTexture2D(NULL), mpRenderTargetView(NULL), mpShaderResourceView(NULL), miHeight(SCREEN_HEIGHT), miWidth(SCREEN_WIDTH)
{
}


RenderTarget::~RenderTarget()
{
	_ASSERT(mpTexture2D == NULL);
	_ASSERT(mpShaderResourceView == NULL);
	_ASSERT(mpRenderTargetView == NULL);
}

bool RenderTarget::Initialise(DirectXDevice* device)
{
	ID3D11Device* Device = device->GetDevice();

	DXGI_SAMPLE_DESC lSamplerDesc = {};
	lSamplerDesc.Count = 1;
	lSamplerDesc.Quality = 0;

	D3D11_TEXTURE2D_DESC bufferDesc = {};
	bufferDesc.ArraySize = 1;
	bufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.Height = miHeight;
	bufferDesc.MipLevels = 1;
	bufferDesc.MiscFlags = 0;
	bufferDesc.SampleDesc = lSamplerDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.Width = miWidth;
	HRESULT hr = Device->CreateTexture2D(&bufferDesc, 0, &mpTexture2D);
	_ASSERT(hr == S_OK);

	if (hr == S_OK)
	{
		//Creating a view of the texture to be used when binding it as a render target
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
		renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		hr = Device->CreateRenderTargetView(mpTexture2D, 0, &mpRenderTargetView);
		_ASSERT(hr == S_OK);

		if (hr == S_OK)
		{
			//Creating a view of the texture to be used when binding it on a shader to sample
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;
			hr = Device->CreateShaderResourceView(mpTexture2D, &shaderResourceViewDesc, &mpShaderResourceView);
			_ASSERT(hr == S_OK);
			
			return true;
		}
	}

	return false;
}

bool RenderTarget::Release()
{
	ULONG refCount = 0;

	if (mpTexture2D)
	{
		refCount = mpTexture2D->Release();
		_ASSERT(refCount == 0);
		mpTexture2D = nullptr;
	}

	if (mpShaderResourceView)
	{
		refCount = mpShaderResourceView->Release();
		_ASSERT(refCount == 0);
		mpShaderResourceView = nullptr;
	}

	if (mpRenderTargetView)
	{
		refCount = mpRenderTargetView->Release();
		_ASSERT(refCount == 0);
		mpRenderTargetView = nullptr;
	}

	return true;
}

void RenderTarget::SetDimensionsToFullscreen()
{
	miWidth = SCREEN_WIDTH;
	miHeight = SCREEN_HEIGHT;
}
