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

#include "Texture.h"
#include "Mesh.h"

// Forward declarations
class DirectXDevice;

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
	// Render Targets
	RenderTarget* mpRenderTargets[RT::Count];

	// DirectX Stuff
	Texture* mpGradientTexture;
	ID3D11SamplerState* mpSamplerState;

	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;

	ID3D11VertexShader* mpVertexShaderPfx;
	ID3D11PixelShader* mpPixelShaderPfx;
	ID3D11InputLayout* mpLayout;

	//D3D Objects To Create Into
	ID3D11Texture2D* mpTexture2D = NULL;
	ID3D11RenderTargetView*	mpRenderTargetView = NULL;
	ID3D11ShaderResourceView* mpShaderResourceView = NULL;

	// Meshes
	Mesh* mpFullscreenQuad;

	// Screen width and height
	int width;
	int height;
	// Which monitor to display on
	int monitor;
	// Window state
	bool mbFullscreen;
	bool mbScreenStateChanged;
	bool mbResolutionChanged;
	bool mbBorderless;
	// Do the post fx pass
	bool mbPostFx;
};

