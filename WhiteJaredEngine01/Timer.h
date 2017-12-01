// Jared White
// March 6, 2017
// WhiteJaredEngine01 Timer.h

#include <ctime>

#pragma once
class Timer
{
private:
	// Data
	clock_t cur;	// Current frame
	//clock_t prev;	// Previous frame
	float prev;
	float t;		// Time in seconds since starting / constructor / declaration
	float dt;		// Time in seconds between the two most recent updates
	float fps;		// Measures 1/dt, approx immediate # of updates per second


public:
	// CONSTRUCTORS

	Timer();
	~Timer();


	// METHODS

	// Update the Timer data
	void update();

	// Get the time passed (in seconds) since the simulation began
	float getTime();

	// Get the change in time (in seconds) since the last frame
	float getDT();

	// Get the number of frames per second that are produced with the last DT
	float getFPS();
};