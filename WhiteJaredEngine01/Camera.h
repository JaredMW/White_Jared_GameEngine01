// Jared White
// April 3, 2017
// WhiteJaredEngine01 Camera.h


#pragma once

#include "Object.h"

//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtx/transform.hpp>
//#include <glm/gtx/euler_angles.hpp>


class Camera
	: public Object
{
private:
	// METHODS
	void move();
	void setFollowDist();
	void setFOV();
	void rotate();

	void updateLookAtMat();
	void updatePerspectiveMat();


	// DATA
	glm::mat4 lookAtMat;	// Translates world; Rotates world around PoV; Simulates a moving, turning camera/eye
	//glm::vec3 camLoc;		// Camera location
	//glm::vec3 camRot;		// Camera rotation
	glm::mat3 rotMat;		// Rotation Matrix
	glm::mat4 worldView; 

	glm::vec3 eye;			// The location that the world is wrapped around and viewed from
	glm::vec3 center;		// The point that the Camera is rotating around
	glm::vec3 up;			// Upwards from the Camera's current location/rotation

	// Movement data
	glm::vec3 velocity;		// Rate of change in camera movement
	float speed;			// Speed of camera movement
	float sensitivity;		// Rate of change in camera turn
	double mouseX, mouseY;	// Cursor location


	// Perspective (Projection) Matrix
	glm::mat4 perspectiveMat;

	float maxFollowDist;	// Don't follow father than this
	float minFollowDist;	// Don't follow closer than this

	float fovy;
	float maxFOV;	// Don't change the fovy beyond this
	float minFOV;	// Don't change the fovy below this
	float aspect;	// Size of the screen
	float zNear;	// Close clipping range
	float zFar;		// Far clipping range

public:
	// DATA
	float followDist;	// Distance that the Camera's eye is from the center
	float zoom;			// The "FOV" coverage this Camera recieves


	// CONSTRUCTORS

	// Create a default Camera Object
	Camera();
	// Create a Camera Object with a specified Transform
	Camera(Transform* transform);
	// Create a Camera Object with a specified Transform
	Camera(glm::vec3& location, glm::vec3& rotation, glm::vec3& scale);
	~Camera();


	// METHODS

	// Setup the Camera
	void start();

	// Update the Camera
	void update();

	// Get the Camera's rotation matrix
	glm::mat3 getRotationMatrix();
};