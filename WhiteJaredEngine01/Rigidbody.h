// Jared White
// May 13, 2017
// WhiteJaredEngine01 Rigidbody.h


#pragma once

#include "Transform.h"
//#include <glm/glm.hpp>
//#include <glm/gtx/transform.hpp>
//#include <glm/gtx/euler_angles.hpp>


// Component for GameObjects that will affect the object's Transform
// with physics operations
class Rigidbody
{
	friend class GameObject; // The GameObject class is friends with the Rigidbody class
							 //		so that it can set the Rigidbody's private Transform
private:
	// DATA
	Transform* transform;		// The Transform that this Rigidbody is attached to
	float drag = -5.f;			// The constant of drag force


	// METHODS

	// Update the position of the Transform
	void updatePosition();


public:
	// DATA
	glm::vec3 velocity;		// The Velocity of this Rigidbody (dist./sec)
	glm::vec3 acceleration;	// The Acceleration of this Rigidbody (dist./sec^2)
	glm::vec3 force;		// The net Forces currently acting on this Rigidbody (F=ma)

	float maxSpeed = 100.f;	// The maximum velocity this Rigidbody can reach
	float mass;				// The Mass of this Rigidbody
	float gravityMultiplier = 0;	// Multiplier of the effects of gravity for this Rigidbody
	bool isActive = true;	// Should this Rigidbody be active and affect its Transform?

	static glm::vec3 gravAccel;		// Universal base acceleration due to gravity
	static float jumpVel;			// The initial value for the jump velocity of all objects


	#pragma region CONSTRUCTORS
	// Create a default Rigidbody
	Rigidbody();

	// Create a Rigidbody with a specified mass
	Rigidbody(float mass);

	// Create a Rigidbody with a specified Transform and mass
	Rigidbody(Transform* transform, float mass = 1.f);
	~Rigidbody();
	#pragma endregion


	// METHODS

	// Update the Transform that this Rigidbody is attached to
	void update();

	// Apply a force to this Rigidbody, directly affecting its acceleration this frame
	void applyForce(glm::vec3 force);

	// Apply a force to this Rigidbody, directly affecting its acceleration this frame
	void applyForce(float x, float y, float z);

	// Apply gravity force to this Rigidbody
	void applyGravity();
	void applyDrag();
};

