// Jared White
// May 13, 2017
// WhiteJaredEngine01 Rigidbody.cpp

#include "Rigidbody.h"
#include "Engine.h"


// Universal acceleration due to gravity
glm::vec3 Rigidbody::gravAccel = glm::vec3(0.f, -.981f, 0.f);
float Rigidbody::jumpVel = 5.f;		// Jump velocity


// Create a default Rigidbody
Rigidbody::Rigidbody()
{
	this->mass = 1.f;
	velocity = glm::vec3(0, 0, 0);
	acceleration = glm::vec3(0, 0, 0);
	force = glm::vec3(0, 0, 0);
}


// Create a Rigidbody with a specified mass
Rigidbody::Rigidbody(float mass)
{
	this->mass = mass;
	velocity = glm::vec3(0, 0, 0);
	acceleration = glm::vec3(0, 0, 0);
	force = glm::vec3(0, 0, 0);
}


// Create a Rigidbody with a Transform and a mass
Rigidbody::Rigidbody(Transform* transform, float mass)
{
	this->transform = transform;
	this->mass = mass;
	velocity = glm::vec3(0, 0, 0);
	acceleration = glm::vec3(0, 0, 0);
	force = glm::vec3(0, 0, 0);
}


Rigidbody::~Rigidbody()
{
}


// Apply a force to the Rigidbody, directly affecting its acceleration
void Rigidbody::applyForce(glm::vec3 force)
{
	// Directly apply force to acceleration
	acceleration = acceleration + (force / Engine::timer.getDT());
}

// Apply a force to the Rigidbody, directly affecting its acceleration
void Rigidbody::applyForce(float x, float y, float z)
{
	acceleration += glm::vec3(x, y, z) / Engine::timer.getDT();
}


// Apply gravitational force to the Rigidbody, scaled by the gravity multiplier
void Rigidbody::applyGravity()
{
	acceleration += gravityMultiplier * mass * gravAccel;
}

void Rigidbody::applyDrag()
{
	force += drag * velocity;
}


// Update the Rigidbody's Transform
void Rigidbody::update()
{
	// Only update if the Rigidbody is active & has a Transform
	if (transform != nullptr && isActive)
	{
		//applyGravity();			// Apply gravitational force
		applyForce(force);		// Apply the net force acting on the Rigidbody
		updatePosition();		// Update the changed final position of the Transform
		force = glm::vec3(0, 0, 0);	// Reset the forces
	}
}


// Update the position of the Transform using calculated data & reset acceleration
void Rigidbody::updatePosition()
{
	// Accumulation of acceleration changes the velocity & clamp to max speed
	velocity += acceleration * Engine::timer.getDT();
	velocity = glm::clamp(velocity, -maxSpeed, maxSpeed);
	

	// Update the location of the object in world space after moving
	transform->location += velocity * Engine::timer.getDT();

	// Reset acceleration
	acceleration = glm::vec3(0, 0, 0);
}