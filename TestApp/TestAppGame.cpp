/**
*  @file MyGame.cpp
*  @brief The main game instance
*
*  A game specific implementation of the parent class "Game.h", loads
*  the assets, runs the game loop, and holds the physics, render, and sound systems.
*
*  @author Sam Murphy
*  @bug No known bugs.
*/

#pragma once

#include "TestAppGame.h"
#include "DirectXDevice.h"
#include "Window_DX.h"

#include <glm/glm.hpp>

#include "PixelShader.h"
#include "VertexShader.h"

#include "PixelShaderPfx.h"
#include "VertexShaderPfx.h"

#include "Hitable.h"
#include "Camera.h"

#include "ImGui\imgui.h"

#include "Log.h"
#include "Globals.h"


TestAppGame::TestAppGame() : Game()
{
}

/**
*  @brief Deletes the render, physics, and sound system
*/
TestAppGame::~TestAppGame()
{
}

void TestAppGame::Initialise(Window_DX * win)
{
	// Init DirectX.
	LOG_INFO << "Initialise the direct X device";
	mpDirectX = new DirectXDevice(win);
	mpDirectX->Initialise(win->GetWidth(), win->GetHeight());

	// Parent init.
	Game::Initialise(win);

	// Create Render Targets
	for (int i = 0; i < RT::Count; i++)
	{
		mpRenderTargets[i] = new RenderTarget();
		mpRenderTargets[i]->SetDimensionsToFullscreen();
		mpRenderTargets[i]->Initialise(mpDirectX);
	}

	monitor = 1;
}

/**
*  @brief Loads all the game assets
*
*
*  Loads the models, textures, materials, fonts, and shaders.
*  Also creates the physics, render, and sound system.
*/
void TestAppGame::LoadAssets()
{
	// Create texture to render
	static const int TextureWidth = 100;
	mpGradientTexture = new Texture();
	mpGradientTexture->SetDimensions(TextureWidth, TextureWidth);
	mpGradientTexture->SetFormat(DXGI_FORMAT_R32G32B32A32_FLOAT);
	mpGradientTexture->SetBindFlags(D3D11_BIND_SHADER_RESOURCE);
	mpGradientTexture->SetCPUAccessFlags(D3D11_CPU_ACCESS_WRITE);
	mpGradientTexture->Initialise(mpDirectX);

	// Create gradient data
	float data[TextureWidth * TextureWidth * 4] = {};
	for (int y = 0; y < TextureWidth; y++)
	{
		for (int x = 0; x < TextureWidth * 4; x += 4)
		{
			int i = (y * TextureWidth * 4) + x;
			data[i]		= x / ((float)TextureWidth * 4.0f);
			data[i + 1] = 1 - (y / ((float)TextureWidth));
			data[i + 2] = 0;
			data[i + 3] = 1.0f;
		}
	}

	mpGradientTexture->CopyDataIntoTexture(mpDirectX, reinterpret_cast<BYTE*>(&data[0]), TextureWidth * 4 * sizeof(float));

	// Create a sampler
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;

	HRESULT result = mpDirectX->GetDevice()->CreateSamplerState(&samplerDesc, &mpSamplerState);

	// GEOMERTRY
	// Init the geometry - full screen quad
	Quad[0] = { 1.0f, -1.0f, 0.0f, 1.0f, 1.0f };
	Quad[1] = { -1.0f, -1.0f, 0.0f, 0.0f, 1.0f };
	Quad[2] = { -1.0f,  1.0f, 0.0f, 0.0f, 0.0f };
	Quad[3] = { 1.0f,  1.0f, 0.0f, 1.0f, 0.0f };
	Quad[4] = { 1.0f, -1.0f, 0.0f, 1.0f, 1.0f };
	Quad[5] = { -1.0f,  1.0f, 0.0f, 0.0f, 0.0f };

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &Quad;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	result = mpDirectX->GetDevice()->CreateBuffer(&bd, &vertexData, &mpVBO);
	_ASSERT(result == S_OK);
	// END OF GEOMETRY
	
	// SHADERS
	// Create shader
	result = mpDirectX->GetDevice()->CreateVertexShader(VertexShader, sizeof(VertexShader), NULL, &mpVertexShader);
	_ASSERT(result == S_OK);
	result = mpDirectX->GetDevice()->CreatePixelShader(PixelShader, sizeof(PixelShader), NULL, &mpPixelShader);
	_ASSERT(result == S_OK);

	result = mpDirectX->GetDevice()->CreateVertexShader(VertexShaderPfx, sizeof(VertexShaderPfx), NULL, &mpVertexShaderPfx);
	_ASSERT(result == S_OK);
	result = mpDirectX->GetDevice()->CreatePixelShader(PixelShaderPfx, sizeof(PixelShaderPfx), NULL, &mpPixelShaderPfx);
	_ASSERT(result == S_OK);

	// set the shader objects
	mpDirectX->GetContext()->VSSetShader(mpVertexShader, 0, 0);
	mpDirectX->GetContext()->PSSetShader(mpPixelShader, 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	result = mpDirectX->GetDevice()->CreateInputLayout(ied, 2, VertexShader, sizeof(VertexShader), &mpLayout);
	_ASSERT(result == S_OK);
	mpDirectX->GetContext()->IASetInputLayout(mpLayout);



	mbFullscreen = false;
	mbScreenStateChanged = false;
	mbResolutionChanged = false;

	mbPostFx = true;

	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;
}

/**
*  @brief Properly end the system threads
*
*
*  Shuts the render and physics system threads down correctly, and calls the game classes shutdown method.
*/
void TestAppGame::Shutdown()
{
	// Clean up Rendertargets
	for (int i = 0; i < RT::Count; i++)
	{
		mpRenderTargets[i]->Release();
		delete mpRenderTargets[i];
	}

	mpDirectX->Shutdown();
	delete mpDirectX;
	Game::Shutdown();
}

void TestAppGame::OnKeypress(int key, bool down)
{
	if (key == 70 && down == true) // F
	{
		mbFullscreen = !mbFullscreen;
		mbBorderless = false;
		mbScreenStateChanged = true;
	}

	if ( key == 66 && down == true ) // B
	{
		mbFullscreen = false;
		mbBorderless = !mbBorderless;
		mbScreenStateChanged = true;
	}

	if (key == 83 && down == true) // S
	{
		SCREEN_WIDTH = (int)width;
		SCREEN_HEIGHT = (int)height;
		mbResolutionChanged = true;
	}

	if (key == 187 && down == true) // +
	{
		width++;
		height++;
	}
	if (key == 189 && down == true) // -
	{
		width--;
		height--;
	}
}

/**
*  @brief Updates the game
*
*/
void TestAppGame::Update(float deltaTime)
{
	//LOG_INFO << "FPS: " << 1.0f / deltaTime;

#if defined D_USE_IMGUI
	if (ImGui::Button("Create Render Target"))
	{
		CreateRenderTarget();
	}

	if (ImGui::Button("Destroy Render Target"))
	{
		DestroyRenderTarget();
	}


	if (ImGui::Button("Go Fullscreen"))
	{
		mbFullscreen = true;
		mbScreenStateChanged = true;
		mbBorderless = false;
	}

	if (ImGui::Button("Go Windowed"))
	{
		mbFullscreen = false;
		mbScreenStateChanged = true;
		mbBorderless = false;
	}

	if ( ImGui::Button( "Go Borderless" ) )
	{
		mbFullscreen = false;
		mbScreenStateChanged = true;
		mbBorderless = true;
	}

	
	ImGui::InputInt("Width", &width);
	ImGui::InputInt("Height", &height);

	static int prevMonitor = 1;
	ImGui::SliderInt("Monitor", &monitor, 1, GetDevice()->GetNumberOfMonitors());
	if (monitor != prevMonitor)
	{
		mbScreenStateChanged = true;
		prevMonitor = monitor;
	}

	if (ImGui::Button("Set Dimensions"))
	{
		SCREEN_WIDTH = (int)width;
		SCREEN_HEIGHT = (int)height;
		mbResolutionChanged = true;
	}

	static bool sbShowDemoWindow = false;
	if (ImGui::Button("Show Demo Window"))
	{
		sbShowDemoWindow = !sbShowDemoWindow;
	}
	if (sbShowDemoWindow)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
		ImGui::ShowDemoWindow(&sbShowDemoWindow);
	}

	if (ImGui::Button("Show Log"))
	{
		GlobalSettings::Settings().renderLog = !GlobalSettings::Settings().renderLog;
	}
	if (GlobalSettings::Settings().renderLog)
	{
		ImGui::SetNextWindowBgAlpha(0.5f);
		GlobalSettings::Settings().imGuiAppLog.Draw("Logger", &GlobalSettings::Settings().renderLog);
	}

	if (ImGui::Button("Toggle PostFx"))
	{
		mbPostFx = !mbPostFx;
	}
#endif
}

/**
*  @brief Render the game
*
*/
void TestAppGame::Render(float deltaTime)
{
	mpDirectX->ClearScreen();

	mpDirectX->EnableDepthBuffering(false);

	// First Pass
	// set the shader objects
	mpDirectX->GetContext()->VSSetShader(mpVertexShader, 0, 0);
	mpDirectX->GetContext()->PSSetShader(mpPixelShader, 0, 0);

	mpDirectX->GetContext()->OMSetRenderTargets(1, mpRenderTargets[ColourBuffer]->GetAddressOfRenderTargetView(), mpDirectX->GetDepthStencilView());

	mpDirectX->GetContext()->PSSetSamplers(0, 1, &mpSamplerState);
	mpDirectX->GetContext()->PSSetShaderResources(0, 1, mpGradientTexture->GetAddressOfShaderResourceView());

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	mpDirectX->GetContext()->IASetVertexBuffers(0, 1, &mpVBO, &stride, &offset);
	// select primitive type
	mpDirectX->GetContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// draw the vertex buffer to the back buffer
	mpDirectX->GetContext()->Draw(6, 0);

	// Second Pass
	if (mbPostFx)
	{
		// set the shader objects
		mpDirectX->GetContext()->VSSetShader(mpVertexShaderPfx, 0, 0);
		mpDirectX->GetContext()->PSSetShader(mpPixelShaderPfx, 0, 0);
		mpDirectX->GetContext()->OMSetRenderTargets(1, mpRenderTargets[PostFx]->GetAddressOfRenderTargetView(), mpDirectX->GetDepthStencilView());

		mpDirectX->GetContext()->PSSetSamplers(0, 1, &mpSamplerState);
		mpDirectX->GetContext()->PSSetShaderResources(0, 1, mpRenderTargets[ColourBuffer]->GetAddressOfShaderResourceView());

		mpDirectX->GetContext()->IASetVertexBuffers(0, 1, &mpVBO, &stride, &offset);
		// select primitive type
		mpDirectX->GetContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// draw the vertex buffer to the back buffer
		mpDirectX->GetContext()->Draw(6, 0);
	}

	// Final Pass
	// set the shader objects
	mpDirectX->GetContext()->VSSetShader(mpVertexShader, 0, 0);
	mpDirectX->GetContext()->PSSetShader(mpPixelShader, 0, 0);

	mpDirectX->GetContext()->OMSetRenderTargets(1, mpDirectX->GetAddressOfBackBuffer(), mpDirectX->GetDepthStencilView());

	mpDirectX->GetContext()->PSSetSamplers(0, 1, &mpSamplerState);
	if (mbPostFx)
		mpDirectX->GetContext()->PSSetShaderResources(0, 1, mpRenderTargets[PostFx]->GetAddressOfShaderResourceView());
	else
		mpDirectX->GetContext()->PSSetShaderResources(0, 1, mpRenderTargets[ColourBuffer]->GetAddressOfShaderResourceView());

	mpDirectX->GetContext()->IASetVertexBuffers(0, 1, &mpVBO, &stride, &offset);
	// select primitive type
	mpDirectX->GetContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// draw the vertex buffer to the back buffer
	mpDirectX->GetContext()->Draw(6, 0);

	// Present
	mpDirectX->SwapBuffers();

	mpDirectX->GetContext()->PSSetShaderResources(0, 1, mpGradientTexture->GetAddressOfShaderResourceView());

	if (mbScreenStateChanged)
	{
		// Clean up Rendertargets
		for (int i = 0; i < RT::Count; i++)
		{
			mpRenderTargets[i]->Release();
		}

		GetDevice()->SetWindowMode(mbFullscreen, mbBorderless, monitor - 1);
		mbScreenStateChanged = false;

		// Recreate up Rendertargets
		for (int i = 0; i < RT::Count; i++)
		{
			mpRenderTargets[i]->Initialise(mpDirectX);
		}
	}

	if (mbResolutionChanged)
	{
		// Clean up Rendertargets
		for (int i = 0; i < RT::Count; i++)
		{
			mpRenderTargets[i]->Release();
		}

		GetDevice()->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		mbResolutionChanged = false;

		// Recreate up Rendertargets
		for (int i = 0; i < RT::Count; i++)
		{
			mpRenderTargets[i]->SetDimensionsToFullscreen();
			mpRenderTargets[i]->Initialise(mpDirectX);
		}
	}
}


void TestAppGame::CreateRenderTarget()
{
	//D3D Device
	ID3D11Device* Device = mpDirectX->GetDevice();

	static UINT lRtWidth = 4096;
	static UINT lRtHeight = lRtWidth;

	DXGI_SAMPLE_DESC lSamplerDesc = {};
	lSamplerDesc.Count = 1;
	lSamplerDesc.Quality = 0;

	D3D11_TEXTURE2D_DESC bufferDesc = {};
	bufferDesc.ArraySize = 1;
	bufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.Height = lRtHeight;
	bufferDesc.MipLevels = 1;
	bufferDesc.MiscFlags = 0;
	bufferDesc.SampleDesc = lSamplerDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.Width = lRtWidth;
	HRESULT hr = Device->CreateTexture2D(&bufferDesc, 0, &mpTexture2D);
	_ASSERT(hr == S_OK);

	//Creating a view of the texture to be used when binding it as a render target
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	hr = Device->CreateRenderTargetView(mpTexture2D, 0, &mpRenderTargetView);
	_ASSERT(hr == S_OK);

	//Creating a view of the texture to be used when binding it on a shader to sample
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	hr = Device->CreateShaderResourceView(mpTexture2D, &shaderResourceViewDesc, &mpShaderResourceView);
	_ASSERT(hr == S_OK);
}

void TestAppGame::DestroyRenderTarget()
{
	mpGradientTexture->Release();
	delete mpGradientTexture;
	mpGradientTexture = nullptr;

	if (mpRenderTargetView)
	{
		mpRenderTargetView->Release();
		mpRenderTargetView = nullptr;
	}
}