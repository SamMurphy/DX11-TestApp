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

#include "GBuffer_PixelShader.h"
#include "GBuffer_VertexShader.h"

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
	mBoostMultiplier = 1.0f;
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
		mpRenderTargets[i]->SetFormat(DXGI_FORMAT_R16G16B16A16_FLOAT);
		mpRenderTargets[i]->Initialise(mpDirectX);
	}


	for (int i = 0; i < GBUFFER_SIZE; i++)
	{
		mpGBuffer[i] = mpRenderTargets[RT::GBufferStart + i]->GetRenderTargetView();
	}
	//mpGBuffer[0] = mpRenderTargets[RT::PositionBuffer]->GetRenderTargetView(); //First target
	//mpGBuffer[1] = mpRenderTargets[RT::NormalBuffer]->GetRenderTargetView(); //second target
	//mpGBuffer[2] = mpRenderTargets[RT::DiffuseBuffer]->GetRenderTargetView(); // third target

	monitor = 1;
	// Create Camera
	mpCamera = new Camera();
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
	// Load the model.
	mpModel = new Model(mpDirectX, "../Resources/Models/Sponza/sponza.obj");
	

	// Create a sampler
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;// D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;

	HRESULT result = mpDirectX->GetDevice()->CreateSamplerState(&samplerDesc, &mpSamplerState);

	// Create full screen quad
	std::vector<Vertex> vertices;
	vertices.push_back(Vertex(1.0f, -1.0f, 0.0f, 1.0f, 1.0f));
	vertices.push_back(Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 1.0f));
	vertices.push_back(Vertex(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex(1.0f, 1.0f, 0.0f, 1.0f, 0.0f));
	vertices.push_back(Vertex(1.0f, -1.0f, 0.0f, 1.0f, 1.0f));
	vertices.push_back(Vertex(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f));
	std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 5 };
	mpFullscreenQuad = new Mesh(vertices, indices);
	mpFullscreenQuad->SetupMesh(mpDirectX);
	
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

	result = mpDirectX->GetDevice()->CreateVertexShader(GBuffer_VertexShader, sizeof(GBuffer_VertexShader), NULL, &mpVertexShaderGBuffer);
	_ASSERT(result == S_OK);
	result = mpDirectX->GetDevice()->CreatePixelShader(GBuffer_PixelShader, sizeof(GBuffer_PixelShader), NULL, &mpPixelShaderGBuffer);
	_ASSERT(result == S_OK);

	// set the shader objects
	mpDirectX->GetContext()->VSSetShader(mpVertexShader, 0, 0);
	mpDirectX->GetContext()->PSSetShader(mpPixelShader, 0, 0);

	// create the input layout object
	int liNumberOfElements = 3;
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	result = mpDirectX->GetDevice()->CreateInputLayout(ied, liNumberOfElements, VertexShader, sizeof(VertexShader), &mpLayout);
	_ASSERT(result == S_OK);
	// Set the input layout
	mpDirectX->GetContext()->IASetInputLayout(mpLayout);

	// Render settings
	mbFullscreen = false;
	mbScreenStateChanged = false;
	mbResolutionChanged = false;
	mbPostFx = true;
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;

	// Create frame buffer
	PerFrameBuffer frameBuffer;

	D3D11_BUFFER_DESC cbDesc1;
	cbDesc1.ByteWidth = sizeof(frameBuffer);
	cbDesc1.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc1.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc1.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc1.MiscFlags = 0;
	cbDesc1.StructureByteStride = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData1;
	InitData1.pSysMem = &frameBuffer;
	InitData1.SysMemPitch = 0;
	InitData1.SysMemSlicePitch = 0;

	// Create the buffer.
	mpDirectX->GetDevice()->CreateBuffer(&cbDesc1, &InitData1, &perFrameBuffer);
}

/**
*  @brief Properly end the system threads
*
*
*  Shuts the render and physics system threads down correctly, and calls the game classes shutdown method.
*/
void TestAppGame::Shutdown()
{
	mpFullscreenQuad->Release();
	delete mpFullscreenQuad;

	mpSamplerState->Release();
	mpSamplerState = nullptr;

	mpVertexShader->Release();
	mpVertexShader = nullptr;

	mpPixelShader->Release();
	mpPixelShader = nullptr;

	mpVertexShaderPfx->Release();
	mpVertexShaderPfx = nullptr;

	mpPixelShaderPfx->Release();
	mpPixelShaderPfx = nullptr;

	mpVertexShaderGBuffer->Release();
	mpVertexShaderGBuffer = nullptr;

	mpPixelShaderGBuffer->Release();
	mpPixelShaderGBuffer = nullptr;

	mpLayout->Release();
	mpLayout = nullptr;

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
	if (mpCamera->OnKeyPress(key, down))
	{
		return;
	}

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
	mpCamera->Update(deltaTime);

#if defined D_USE_IMGUI
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

	ImGui::InputFloat("Boost", &mBoostMultiplier);
#endif
}

/**
*  @brief Render the game
*
*/
void TestAppGame::Render(float deltaTime)
{
	
	// Clear the screen
	mpDirectX->ClearScreen();
	for (int i = 0; i < RT::Count; i++)
	{
		mpRenderTargets[i]->Clear(mpDirectX);
	}

	mpDirectX->EnableDepthBuffering(true);
	mpDirectX->EnableAlphaBlending(false);

	// First Pass
	// set the shader objects
	mpDirectX->GetContext()->VSSetShader(mpVertexShaderGBuffer, 0, 0);
	mpDirectX->GetContext()->PSSetShader(mpPixelShaderGBuffer, 0, 0);

	//mpDirectX->GetContext()->OMSetRenderTargets(1, mpRenderTargets[ColourBuffer]->GetAddressOfRenderTargetView(), mpDirectX->GetDepthStencilView());
	mpDirectX->GetContext()->OMSetRenderTargets(GBUFFER_SIZE, mpGBuffer, mpDirectX->GetDepthStencilView());

	mpDirectX->GetContext()->PSSetSamplers(0, 1, &mpSamplerState);

	// Set camera
	glm::vec4 cameraPosition = glm::vec4(0, 0, 0, 1);
	PerFrameBuffer frameBuffer;
	memcpy(&frameBuffer.PM, &mpCamera->GetProjectionMatrix()[0][0], sizeof(glm::mat4x4));
	memcpy(&frameBuffer.PM_Inv, &(glm::inverse(mpCamera->GetProjectionMatrix()))[0][0], sizeof(glm::mat4x4));
	memcpy(&frameBuffer.VM, &mpCamera->GetViewMatrix()[0][0], sizeof(glm::mat4x4));
	memcpy(&frameBuffer.VM_Inv, &(glm::inverse(mpCamera->GetViewMatrix()))[0][0], sizeof(glm::mat4x4));
	memcpy(&frameBuffer.CameraPosition, &cameraPosition[0], sizeof(glm::vec4));

	D3D11_MAPPED_SUBRESOURCE ms1;
	mpDirectX->GetContext()->Map(perFrameBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms1);
	memcpy(ms1.pData, &frameBuffer, sizeof(PerFrameBuffer));
	mpDirectX->GetContext()->Unmap(perFrameBuffer, NULL);
	mpDirectX->GetContext()->VSSetConstantBuffers(1, 1, &perFrameBuffer);
	mpDirectX->GetContext()->PSSetConstantBuffers(1, 1, &perFrameBuffer);

	// Draw the model
	mpModel->Draw(mpDirectX);

	mpDirectX->EnableDepthBuffering(false);
	mpDirectX->EnableAlphaBlending(false);
	// Post FX pass
	if (mbPostFx)
	{
		// set the shader objects
		mpDirectX->GetContext()->VSSetShader(mpVertexShaderPfx, 0, 0);
		mpDirectX->GetContext()->PSSetShader(mpPixelShaderPfx, 0, 0);
		mpDirectX->GetContext()->OMSetRenderTargets(1, mpRenderTargets[PostFx]->GetAddressOfRenderTargetView(), mpDirectX->GetDepthStencilView());

		mpDirectX->GetContext()->PSSetSamplers(0, 1, &mpSamplerState);
		mpDirectX->GetContext()->PSSetShaderResources(0, 1, mpRenderTargets[DiffuseBuffer]->GetAddressOfShaderResourceView());

		//mpFullscreenQuad->GetVBO()->Draw(mpDirectX);
		mpFullscreenQuad->Draw(mpDirectX);
	}

	// Final Pass - Copy pfx or colour buffer to the back buffer
	// set the shader objects
	mpDirectX->GetContext()->VSSetShader(mpVertexShaderPfx, 0, 0);
	mpDirectX->GetContext()->PSSetShader(mpPixelShader, 0, 0);

	mpDirectX->GetContext()->OMSetRenderTargets(1, mpDirectX->GetAddressOfBackBuffer(), mpDirectX->GetDepthStencilView());

	mpDirectX->GetContext()->PSSetSamplers(0, 1, &mpSamplerState);
	if (mbPostFx)
		mpDirectX->GetContext()->PSSetShaderResources(0, 1, mpRenderTargets[PostFx]->GetAddressOfShaderResourceView());
	else
		mpDirectX->GetContext()->PSSetShaderResources(0, 1, mpRenderTargets[DiffuseBuffer]->GetAddressOfShaderResourceView());

	//mpFullscreenQuad->GetVBO()->Draw(mpDirectX);
	mpFullscreenQuad->Draw(mpDirectX);

	// Present
	mpDirectX->SwapBuffers();


	// -- HANDLE CHANGE OF SCREEN STATE OR RESOLUTION --

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

		GetDevice()->SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
		mbResolutionChanged = false;

		// Recreate up Rendertargets
		for (int i = 0; i < RT::Count; i++)
		{
			mpRenderTargets[i]->SetDimensionsToFullscreen();
			mpRenderTargets[i]->Initialise(mpDirectX);
		}
	}
}