// Jared White
// March 6, 2017
// WhiteJaredEngine01 Timer.cpp


#include <string>
#include "Timer.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>



Timer::Timer()
{
	//prev = cur = clock();
	t = prev = dt = fps = 0;
}


Timer::~Timer()
{
}

// Update Timer data
void Timer::update()
{
	//prev = cur;									// Update the previous frame
	////cur = clock();							// Update the current time frame
	//dt = (cur - prev) / (float)CLOCKS_PER_SEC;	// Delta time since last frame
	//t += dt;									// The current time
	//fps = 1 / dt;								// Frames per second

	prev = t;
	t = (float)glfwGetTime();
	dt = t - prev;
	fps = 1 / dt;
}

// Get the time passed (in seconds) since the start of the simulation
float Timer::getTime()
{
	return t;
}

// Get the change in time since the last frame (in seconds)
float Timer::getDT()
{
	return dt;
}

// Get the current Frames Per Second
float Timer::getFPS()
{
	return fps;
}