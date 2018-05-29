/**
*  @file Game.h
*  @brief Class for specific game to inherit from.
*
*  Specific games should inherit from this to create there instance.
*
*  @author Sam Murphy
*  @bug No known bugs.
*/
#pragma once
#include "stdafx.h"
#include <vector>
#include "Timer.h"

// Forward Declarations
class Window_DX;
class DirectXDevice;

/**
*  @brief Class for specific game to inherit from.
*
*  Holds the managers and systems that the game will need.
*/
class Game
{
// Data
private:
	/// The window of the game.
	Window_DX* gameWindow;
	/// The games timer, used for deltaTime.
	Timer timer;

protected:
	/// If set to true, the game loop will end.
	bool quitFlag;
	DirectXDevice* mpDirectX;
	
// Constructors
public:
	Game();
	~Game();
private:
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

// Getters & Setters
public:
	float GetTime() const	{ return timer.DeltaTime(); }
	bool GetQuitFlag() const { return quitFlag; }
	void SetQuitFlag(bool b) { quitFlag = b; }
	Window_DX* GetWindow()	{ return gameWindow; }
	Timer* GetTimer() { return &timer; }
	DirectXDevice* GetDevice() { return mpDirectX; }

// Functions
public:
	virtual void Initialise(Window_DX* win);
	virtual void LoadAssets() = 0;
	virtual void Run();
	virtual void Update(float deltaTime) = 0;
	virtual void Render(float deltaTime) = 0;
	virtual void Shutdown();
	
	virtual void OnKeypress(int key, bool down);
	virtual void OnMouseDown(float x, float y, int button, bool down);
	virtual void OnMouseMove(float x, float y);
};

