/**
*  @file Timer.cpp
*  @brief Timer used to get the deltaTime for the games and threaded systems
*
*  Timer class used to create and access a deltaTime value for a game loop, or threaded systems.

*  @author Sam Murphy
*  @bug No known bugs.
*/

#include "Timer.h"



Timer::Timer() : currentTime(0), deltaTime(0)
{
}


Timer::~Timer()
{
}

/**
*  @brief Updates the deltaTime
*  
*  Calculates the new deltaTime from how long it's been since UpdateTime() was last called.
*/
void Timer::UpdateTime()
{
	double tempTime = clock();
	deltaTime = (tempTime - currentTime) / CLOCKS_PER_SEC;
	currentTime = tempTime;
}