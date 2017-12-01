// Jared White
// May 16, 2017
// WhiteJaredEngine01 Script.h


#pragma once
#include "Transform.h"

// A Class to represent a varying set of behaviors
class Script
{
	friend class GameObject;
protected:
	// DATA
	Transform* transform;	// The Transform that this Script is attached to


	// METHODS

	// On creation of the object, execute these events
	virtual void start() = 0;
	// Each frame of the simulation, execute this series of events
	virtual void update() = 0;
	// On end of the object, execute these events
	virtual void stop() = 0;

public:
	// DATA
	bool isActive = true;


	// CONSTRUCTORS 

	Script();
	virtual ~Script();
	

	// METHODS

	// Get the Transform that this Script is attached to
	Transform* getTransform();
};