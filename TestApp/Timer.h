/**
*  @file Timer.h
*  @brief Timer used to get the deltaTime for the games and threaded systems
*
*  Timer class used to create and access a deltaTime value for a game loop, or threaded systems.

*  @author Sam Murphy
*  @bug No known bugs.
*/

#pragma once
#include "stdafx.h"
#include <windows.h>
#include <time.h>

/**
*  @brief Timer used to get the deltaTime for the games and threaded systems
*  
*  Used to create and access a deltaTime value for a game loop, or threaded systems.
*/
class Timer
{
public:
	Timer();
	~Timer();

	void UpdateTime();

	double DeltaTime() const { return deltaTime; }

private:
	/// The current time as a double.
	double currentTime;
	/// The most recently calculated delta time.
	double deltaTime;
};

