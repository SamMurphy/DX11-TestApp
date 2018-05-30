/**
*  @file MyGame.h
*  @brief The main game instance
*
*
*  @author Sam Murphy
*  @bug No known bugs.
*/

#pragma once
#include "Game.h"
#include <stdlib.h>

#include <d3d11.h>
#include <glm/glm.hpp>

#include "RenderTarget.h"
#include <vector>

// Forward declarations
class DirectXDevice;

struct Vertex
{
	float x, y, z;
	float u, v;
};

enum RT
{
	ColourBuffer,
	PostFx,

	Count
};

/**
* A game specific implementation of the parent class "Game.h", loads
* the assets, runs the game loop, and holds the physics, render, and sound systems.
*
*/
class TestAppGame :
	public Game
{
public:
	TestAppGame();
	virtual ~TestAppGame();

public:
	// Public Functions
	virtual void Initialise(Window_DX* win);
	virtual void LoadAssets();
	virtual void Shutdown();
	virtual void OnKeypress(int key, bool down);

	virtual void Update(float deltaTime);

	virtual void Render(float deltaTime);

	float Random() { return ((double)rand() / (RAND_MAX)); }

	void SetFullscreen(const bool lbFullscreen) { mbFullscreen = lbFullscreen; mbScreenStateChanged = true; }
	const bool GetFullscreen() const { return mbFullscreen; }

	void CreateRenderTarget();
	void DestroyRenderTarget();

private:
	// Private Functions

private:
	// Data
	Vertex Quad[6];

	RenderTarget* mpRenderTargets[RT::Count];

	// DirectX Stuff
	ID3D11Texture2D* mpTexture;
	ID3D11ShaderResourceView* mpTextureSRV;
	ID3D11SamplerState* mpSamplerState;

	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;
	ID3D11InputLayout* mpLayout;
	ID3D11Buffer* mpVBO;

	ID3D11VertexShader* mpVertexShaderPfx;
	ID3D11PixelShader* mpPixelShaderPfx;

	//D3D Objects To Create Into
	ID3D11Texture2D* mpTexture2D = NULL;
	ID3D11RenderTargetView*	mpRenderTargetView = NULL;
	ID3D11ShaderResourceView* mpShaderResourceView = NULL;

	int width;
	int height;

	int monitor;

	bool mbFullscreen;
	bool mbScreenStateChanged;
	bool mbResolutionChanged;
	bool mbBorderless;

	bool mbPostFx;
};

