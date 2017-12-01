// Jared White
// May 3, 2017
// WhiteJaredEngine01 Transform.h

#pragma once

//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>


// Represents the location, rotation, and scale
// of an object in a 3-dimensional space
class Transform
{
private:
	// DATA
	glm::mat4 localWorldMat;	// Matrix to transform local space coordinates to world space coordinates
	glm::mat3 rotationMatrix;	// Matrix to represent Transform rotation
	glm::vec3 forward;			// The forward vector of this Transform

public:
	// DATA
	glm::vec3 location;		// The location of this Transform in world space
	glm::vec3 rotation;		// The rotation of this Transform in world space
	glm::vec3 scale;		// The size of what is attached to this Transform in world space


	#pragma region CONSTRUCTORS
	// Create a Transform with default values
	Transform();

	// Create a Transform with specified values
	Transform(glm::vec3& location, glm::vec3& rotation, glm::vec3& scale = glm::vec3(1, 1, 1));

	~Transform();
	#pragma endregion


	#pragma region METHODS
	// Set the components of this Transform's rotation (in radians)
	void setRotation(float x, float y, float z);
	// Set the components of this Transform's rotation (in radians)
	void setRotation(glm::vec3 rotation);

	// Get the rotation of this Transform
	glm::vec3 getRotation() const;

	// Get the forward vector of this Transform
	glm::vec3 getForward() const;

	// Get the rotation matrix of this Transform
	glm::mat3 getRotationMat() const;

	// Get this Transform's Matrix which transforms a point from its local
	// space to world space.
	glm::mat4 getLocalWorldMat() const;

	// Update this Transform by updating the local-world matrix
	void update();
	#pragma endregion
};