// Jared White
// May 16, 2017
// WhiteJaredEngine01 MovementScript.cpp


#pragma once
#include "Script.h"
#include "Rigidbody.h"
#include "Camera.h"


// Allows movement of a Transform utilizing the WASD keys
class MovementScript :
	public Script
{
	friend class Engine;
protected:
	// DATA
	glm::vec3 force;		// Velocity to move the Transform at
	Camera* camera;			// The Camera attached to this moving object
	Rigidbody* rigidbody;	// The Rigidbody of this moving object

	bool jumpPresPrev = false;
	bool jumpPresCur = false;


	// METHODS

	void start();
	void update();
	void stop();

	void move();
	void jump();


public:
	// DATA
	float speed = 1.f;	// Speed to apply Force to Rigidbody with


	// CONSTRUCTORS

	MovementScript();
	MovementScript(Camera* camera, float speed, Rigidbody* rigidbody);
	~MovementScript();
};