/**
*  @file Game.cpp
*  @brief Class for specific game to inherit from.
*
*  Specific games should inherit from this to create there instance.
*
*  @author Sam Murphy
*  @bug No known bugs.
*/
#pragma once

#include "Game.h"
#include "Window_DX.h"
#include "Log.h"
/*----------------------------------------------------------------------------------------------------------------*/
// CONSTRUCTORS
/*----------------------------------------------------------------------------------------------------------------*/

Game::Game()
	: timer(), quitFlag(false)
{
}

/*----------------------------------------------------------------------------------------------------------------*/

Game::~Game()
{
}

/**
*  @brief Safely releases the games resources. 
*
*  Calls shutdown on all the managers and resources.
*  
*/
void Game::Shutdown()
{
}

/*----------------------------------------------------------------------------------------------------------------*/
// FUNCTIONS
/*----------------------------------------------------------------------------------------------------------------*/


/**
*  @brief Initialises the Game.
*
*  Sets the window, and creates the GraphicsDevice, SoundDevice, ResourceManager, InputManager,
*  NetworkingManager, SceneManager and then calls the LoadAssets() method.
*  @param win The games window.
*/
void Game::Initialise(Window_DX* win)
{
	gameWindow = win;

	LoadAssets();
}

/**
*  @brief Called by the game loop, every iteration.
*
*  Updates the deltaTime and calls the update method.
*/
void Game::Run()
{
	timer.UpdateTime();

	// Do the update logic.
	Update(timer.DeltaTime());
	// Render the game.
	Render(timer.DeltaTime());
}

/**
*  @brief Called when a key is pressed.
*
*  Passes the key press into the InputManager.
*
*  @param key The key that's been pressed.
*  @param down True if the key has been pressed down, false if released.
*/
void Game::OnKeypress(int key, bool down)
{
}

/**
*  @brief Called when a mouse button is pressed.
*
*  Passes the mouse press into the InputManager.
*  
*  @param x The x coordinate of the mouse.
*  @param y The y coordinate of the mouse.
*  @param button The button that has been pressed.
*  @param down Whether the button has been pressed down or not.
*/
void Game::OnMouseDown(float x, float y, int button, bool down)
{
}

/**
*  @brief Called when the mouse moves.
*
*  Passes the mouse coordinates into the InputManager.
*
*  @param x The x coordinate of the mouse.
*  @param y The y coordinate of the mouse.
*/
void Game::OnMouseMove(float x, float y)
{
}