#pragma once
#include "RenderTarget.h"
#include "Globals.h"
#include <crtdbg.h>  

static const float clearColourRT[4] = { 227.0f / 255.0f, 0, 140.0f / 255.0f, 1.0f }; // Pinkish

RenderTarget::RenderTarget() : Texture(),  mpRenderTargetView(NULL)
{
	meUsage = D3D11_USAGE_DEFAULT;
}

RenderTarget::~RenderTarget()
{
	_ASSERT(mpRenderTargetView == NULL);
}

bool RenderTarget::Initialise(DirectXDevice* device)
{
	miBindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	miCPUAccessFlags = 0;
	meUsage = D3D11_USAGE_DEFAULT;
	if (Texture::Initialise(device))
	{
		ID3D11Device* Device = device->GetDevice();

		//Creating a view of the texture to be used when binding it as a render target
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
		renderTargetViewDesc.Format = mFormat;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		HRESULT hr = Device->CreateRenderTargetView(mpTexture, 0, &mpRenderTargetView);
		_ASSERT(hr == S_OK);
		return (hr == S_OK);
	}
	return false;
}

bool RenderTarget::Release()
{
	ULONG refCount = 0;

	Texture::Release();

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

void RenderTarget::Clear(DirectXDevice * device)
{
	device->GetContext()->ClearRenderTargetView(mpRenderTargetView, clearColourRT);
}
