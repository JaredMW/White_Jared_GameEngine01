// Jared White
// April 3, 2017
// WhiteJaredEngine01 Camera.cpp

#include "Camera.h"
#include "Engine.h"

#define _USE_MATH_DEFINES
#include <math.h>
//#define M_PI 3.14159265358979323846f	// pi

//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//
//#include <glm/gtx/transform.hpp>
//#include <glm/gtx/euler_angles.hpp>


Camera::Camera()
{
	tag = "Camera";
}

// Create a Camera object with a specified Transform
Camera::Camera(Transform* transform)
{
	tag = "Camera";
	this->transform = transform;
}

// Create a Camera object with a specified Transform
Camera::Camera(glm::vec3& location, glm::vec3& rotation, glm::vec3& scale)
{
	tag = "Camera";
	transform = &Transform(location, rotation, scale);
}

Camera::~Camera()
{
}


// FPS controls: Move the camera with arrow keys
void Camera::move()
{
	velocity = glm::vec3();

	#pragma region Projection-Based Movement Controls
	// Left movement
	if (glfwGetKey(Engine::windowPtr, GLFW_KEY_LEFT)
		|| glfwGetKey(Engine::windowPtr, GLFW_KEY_A))
	{
		//dir.x -= 1;
		velocity += rotMat * glm::vec3(-1, 0, 0);
	}
	// Right movement
	if (glfwGetKey(Engine::windowPtr, GLFW_KEY_RIGHT)
		|| glfwGetKey(Engine::windowPtr, GLFW_KEY_D))
	{
		//dir.x += 1;
		velocity += rotMat * glm::vec3(1, 0, 0);
	}
	// Forward movement
	if (glfwGetKey(Engine::windowPtr, GLFW_KEY_UP)
		|| glfwGetKey(Engine::windowPtr, GLFW_KEY_W))
	{
		//dir.z -= 1;
		velocity += rotMat * glm::vec3(0, 0, -1);
	}
	// Backwards movement
	if (glfwGetKey(Engine::windowPtr, GLFW_KEY_DOWN)
		|| glfwGetKey(Engine::windowPtr, GLFW_KEY_S))
	{
		//dir.z += 1;
		velocity += rotMat * glm::vec3(0, 0, 1);
	}
	#pragma endregion


	// Ensure direction isn't a zero vector so it can be normalized w/o errors.
	// Then, scale projection-based movement to speed of movement.
	if (velocity/*dir*/ != glm::vec3())
	{
		velocity/*dir*/ = glm::normalize(velocity/*dir*/) * speed;
	}


	#pragma region World-Based Y-Movement Controls
	// World-Based Upwards movement
	if (glfwGetKey(Engine::windowPtr, GLFW_KEY_E)
		|| glfwGetKey(Engine::windowPtr, GLFW_KEY_PAGE_UP))
	{
		/*dir*/velocity.y += 1;
	}
	// World-Based Downwards movement
	if (glfwGetKey(Engine::windowPtr, GLFW_KEY_Q)
		|| glfwGetKey(Engine::windowPtr, GLFW_KEY_PAGE_DOWN))
	{
		/*dir*/velocity.y -= 1;
	}
	#pragma endregion

	//velocity = /*rotMat * */dir/* * speed*/;			// Update velocity w/ final velocity vector
	transform->location += Engine::timer.getDT() * velocity;	// Update the location of camera based on time


	//#pragma region Perspective Distance Controls
	//// Update Follow Dist (1st vs. 3rd person)
	//// Closer
	//if (glfwGetKey(Engine::windowPtr, GLFW_KEY_Z)
	//	&& followDist > 0)
	//{
	//	followDist -= 1.f * Engine::timer.getDT();
	//	if (followDist < 0)
	//	{
	//		followDist = 0;
	//	}
	//}
	//// Farther away
	//if (glfwGetKey(Engine::windowPtr, GLFW_KEY_X)
	//	&& followDist < 10.f)
	//{
	//	followDist += 1.f * Engine::timer.getDT();
	//	if (followDist > 10.f)
	//	{
	//		followDist = 10.f;
	//	}
	//}
	//#pragma endregion

	
	//#pragma region Field of View Controls
	//// Zoom Out w/ Right Mouse
	//if (glfwGetMouseButton(Engine::windowPtr, GLFW_MOUSE_BUTTON_2)
	//	&& zoom > .41f)
	//{
	//	zoom -= 1.f * Engine::timer.getDT();
	//	if (zoom < .41f)
	//	{
	//		zoom = .41f;
	//	}
	//}
	//// Zoom In w/ Left Mouse
	//if (glfwGetMouseButton(Engine::windowPtr, GLFW_MOUSE_BUTTON_1)
	//	&& zoom < 1.f)
	//{
	//	zoom += 1.f * Engine::timer.getDT();
	//	if (zoom > 1.f)
	//	{
	//		zoom = 1.f;
	//	}
	//}

	//// Update perspective matrix
	//fovy = (float)M_PI * .4f / zoom;
	//perspectiveMat = glm::perspective(fovy, aspect, zNear, zFar);
	//#pragma endregion
}


// Update the camera's follow distance behind the eye
void Camera::setFollowDist()
{
	// Update Follow Dist (1st vs. 3rd person)
	// Closer
	if (glfwGetKey(Engine::windowPtr, GLFW_KEY_Z)
		&& followDist > minFollowDist)
	{
		followDist -= 1.f * Engine::timer.getDT();
		if (followDist < minFollowDist)
		{
			followDist = minFollowDist;
		}
	}
	// Farther away
	if (glfwGetKey(Engine::windowPtr, GLFW_KEY_X)
		&& followDist < maxFollowDist)
	{
		followDist += 1.f * Engine::timer.getDT();
		if (followDist > maxFollowDist)
		{
			followDist = maxFollowDist;
		}
	}
}


// Update field-of-view controls
void Camera::setFOV()
{
	// Zoom Out w/ Right Mouse
	if (glfwGetMouseButton(Engine::windowPtr, GLFW_MOUSE_BUTTON_2)
		&& zoom > minFOV)
	{
		zoom -= 1.f * Engine::timer.getDT();
		if (zoom < minFOV)
		{
			zoom = minFOV;
		}
	}
	// Zoom In w/ Left Mouse
	if (glfwGetMouseButton(Engine::windowPtr, GLFW_MOUSE_BUTTON_1)
		&& zoom < maxFOV)
	{
		zoom += 1.f * Engine::timer.getDT();
		if (zoom > maxFOV)
		{
			zoom = maxFOV;
		}
	}
}


// Update the Perspective matrix
void Camera::updatePerspectiveMat()
{
	fovy = (float)M_PI * .4f / zoom;
	perspectiveMat = glm::perspective(fovy, aspect, zNear, zFar);
}


// Rotate the Camera
void Camera::rotate()
{
	// Get position of cursor
	glfwGetCursorPos(Engine::windowPtr, &mouseX, &mouseY);

	// Displace camera rotation by difference in mouse's position from the center of the screen
	transform->rotation.y -= sensitivity * (float)(mouseX - Engine::midWidth);  // Yaw
	transform->rotation.x -= sensitivity * (float)(mouseY - Engine::midHeight); // Pitch
	transform->rotation.x = glm::clamp(transform->rotation.x, -.305f * (float)M_PI, 0.159f);	// Clamp rotation

	// Mouse turn: Update the rotation matrix
	rotMat = (glm::mat3)glm::yawPitchRoll(transform->rotation.y, transform->rotation.x, transform->rotation.z);
}


// Update the look-at matrix
void Camera::updateLookAtMat()
{
	// Update look-at matrix
	// 1st vs 3rd person
	if (followDist == 0)
	{
		eye = transform->location;
	}
	else
	{
		eye = transform->location + rotMat * glm::vec3(0.f, 0.f, followDist);
	}

	center = transform->location + glm::vec3(0.f, 0.8f, 0.f) + rotMat * glm::vec3(0, 0, -1);
	up = rotMat * glm::vec3(0.f, 1.f, 0.f);
	lookAtMat = glm::lookAt(eye, center, up);

	// Reset the cursor to the center of the screen
	//glfwSetCursorPos(Engine::windowPtr, Engine::midWidth, Engine::midHeight);
}


// Setup the camera & initialize values
void Camera::start()
{
	// Setup the world view matrix
	worldView = glm::mat4();

	// Setup Camera transform
	//transform->location = { 0, 0, 2 };	// Initial camera location
	//transform->rotation = { 0, 0, 0 };	// Initial camera rotation

	rotMat = (glm::mat3)glm::yawPitchRoll(transform->rotation.y, transform->rotation.x, transform->rotation.z);

	// Setup the look-at matrix
	followDist = 0.f;
	minFollowDist = 0.f;
	maxFollowDist = 10.f;

	updateLookAtMat();
	//eye = transform.location;
	//center = eye + rotMat * glm::vec3(0, 0, -1);
	//up = rotMat * glm::vec3(0, 1, 0);
	//lookAtMat = glm::lookAt(eye, center, up);	// Set the look-at matrix


	// Setup the Perspective (Projection) Matrix
	zoom = .85f;
	fovy = (float)M_PI * .4f / zoom;
	minFOV = .41f;
	maxFOV = 1.f;
	aspect = (float)Engine::width / (float)Engine::height;
	zNear = .01f;
	zFar = 1000.f;

	perspectiveMat = glm::perspective(fovy, aspect, zNear, zFar);

	// Movement
	sensitivity = .005f;
	speed = 1.f;

	// Set cursor position to center of the screen
	glfwSetCursorPos(Engine::windowPtr, Engine::midWidth, Engine::midHeight);
}


// Update the camera to check for movement and rotation input
void Camera::update()
{
	// Move & rotate the camera
	//move();
	//setFollowDist();
	//setFOV();
	rotate();

	updatePerspectiveMat();
	updateLookAtMat();

	worldView = perspectiveMat * lookAtMat;
		//* glm::lookAt(
		//	eye,//camLoc,
		//	center,//camLoc + rotMat * glm::vec3(0, 0, -1),
		//	up/*rotMat * glm::vec3(0, 1, 0)*/);

	// Set cursor position to center of the screen
	glfwSetCursorPos(Engine::windowPtr, Engine::midWidth, Engine::midHeight);

	glUniformMatrix4fv(Engine::shader.shaderWorldViewMatLoc, 1, GL_FALSE, &worldView[0][0]);
	glUniform3f(Engine::shader.shaderCameraLoc, eye.x, eye.y, eye.z);
}


// Get the Camera's rotation matrix
glm::mat3 Camera::getRotationMatrix()
{
	return rotMat;
}
