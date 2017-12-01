// Jared White
// May 3, 2017
// WhiteJaredEngine01 Transform.cpp

#include "Transform.h"


// Create a Transform with default values
Transform::Transform()
{
	location = glm::vec3(0, 0, 0);	// Set the default location	
	rotation = glm::vec3(0, 0, 0);	// Set the default rotation
	scale = glm::vec3(1, 1, 1);		// Set the default scale
	forward = glm::vec3(1, 0, 0);	// Set the default forward

	// Set the default local-to-world matrix
	localWorldMat = glm::translate(location)
		* glm::yawPitchRoll(rotation.y, rotation.x, rotation.z)
		* glm::scale(scale);
}

// Create a Transform with specified values
Transform::Transform(glm::vec3& location, glm::vec3& rotation, glm::vec3& scale)
{
	this->location = location;	// Set the location
	this->rotation = rotation;	// Set the rotation
	this->scale = scale;		// Set the scale

	// Set the rotation matrix & rotated normalized forward vector using that matrix
	rotationMatrix = (glm::mat3)glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
	forward = rotationMatrix * glm::vec3(1.f, 0, 0);

	// Matrix multiplication has the associative property:
	// worldVertLoc = translate * rotate * scale * objVertLoc;
	localWorldMat = glm::translate(location)
		* glm::yawPitchRoll(rotation.y, rotation.x, rotation.z)
		* glm::scale(scale);
}

Transform::~Transform()
{
}


// Set the rotation of this Transform
void Transform::setRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
	rotationMatrix = (glm::mat3)glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
	forward = rotationMatrix * forward;
}

// Set the rotation of this Transform
void Transform::setRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
	rotationMatrix = (glm::mat3)glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
	forward = rotationMatrix * forward;
}

// Get the rotation of this Transform
glm::vec3 Transform::getRotation() const
{
	return rotation;
}

// Get the forward vector of this Transform
glm::vec3 Transform::getForward() const
{
	return forward;
}

// Get the rotation matrix of this Transform
glm::mat3 Transform::getRotationMat() const
{
	return rotationMatrix;
}

// Return the matrix that transforms a point from local to world space.
glm::mat4 Transform::getLocalWorldMat() const
{
	return localWorldMat;
}


// Update this Transform
void Transform::update()
{
	// Matrix multiplication has the associative property:
	//		worldVertLoc = translate * rotate * scale * localVertLoc;
	localWorldMat = glm::translate(location)
		* glm::yawPitchRoll(rotation.y, rotation.x, rotation.z)
		* glm::scale(scale);

	rotationMatrix = (glm::mat3)glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
}