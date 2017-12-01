// Jared White
// May 16, 2017
// WhiteJaredEngine01 MovementScript.cpp

#include "MovementScript.h"
#include "Engine.h"


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

MovementScript::MovementScript()
{
}

MovementScript::MovementScript(Camera* camera, float speed, Rigidbody* rigidbody)
{
	this->camera = camera;
	this->speed = speed;
	this->rigidbody = rigidbody;
}


MovementScript::~MovementScript()
{
}


// Start the Script
void MovementScript::start()
{
	rigidbody->gravityMultiplier = 1.f;
}


// Update this Script
void MovementScript::update()
{
	// JUMPING TAKEN OUT OF GAME

	/*jumpPresPrev = jumpPresCur;
	jumpPresCur = false;

	jump();*/
	//rigidbody->applyGravity();	// Do this in engine logic
	//rigidbody->applyDrag();	// Don't need drag - game is on ice
	move();
}


// Stop the Script
void MovementScript::stop()
{
}


// Move the Transform this Script is attached to
void MovementScript::move()
{
	// Set the rotation of the transform based on the camera's rotation
	transform->setRotation(0.f, camera->transform->getRotation().y, 0.f);

	// Create a total force
	force = glm::vec3();

	#pragma region Projection-Based XZ-Plane Movement Controls
	// Left movement
	if (glfwGetKey(Engine::windowPtr, GLFW_KEY_LEFT)
		|| glfwGetKey(Engine::windowPtr, GLFW_KEY_A))
	{
		//velocity += transform->getRotationMat() * glm::vec3(-1, 0, 0);
		force += (glm::mat3)glm::yawPitchRoll(camera->transform->getRotation().y, 0.f, 0.f) * glm::vec3(-1.f, 0.f, 0.f);
	}
	// Right movement
	if (glfwGetKey(Engine::windowPtr, GLFW_KEY_RIGHT)
		|| glfwGetKey(Engine::windowPtr, GLFW_KEY_D))
	{
		force += (glm::mat3)glm::yawPitchRoll(camera->transform->getRotation().y, 0.f, 0.f) * glm::vec3(1.f, 0.f, 0.f);
	}
	// Forward movement
	if (glfwGetKey(Engine::windowPtr, GLFW_KEY_UP)
		|| glfwGetKey(Engine::windowPtr, GLFW_KEY_W))
	{
		//velocity += transform->getRotationMat() * glm::vec3(0, 0, -1);
		force += (glm::mat3)glm::yawPitchRoll(camera->transform->getRotation().y, 0.f, 0.f) * glm::vec3(0.f, 0.f, -1.f);
	}
	// Backwards movement
	if (glfwGetKey(Engine::windowPtr, GLFW_KEY_DOWN)
		|| glfwGetKey(Engine::windowPtr, GLFW_KEY_S))
	{
		force += (glm::mat3)glm::yawPitchRoll(camera->transform->getRotation().y, 0.f, 0.f) * glm::vec3(0.f, 0.f, 1.f);
	}
	#pragma endregion


	// Ensure direction isn't a zero vector so it can be normalized w/o errors.
	// Then, scale projection-based movement to speed of movement.
	if (force != glm::vec3())
	{
		force = glm::normalize(force) * speed;
	}


	//#pragma region World-Based Y-Movement Controls
	//// World-Based Upwards movement
	//if (glfwGetKey(Engine::windowPtr, GLFW_KEY_E)
	//	|| glfwGetKey(Engine::windowPtr, GLFW_KEY_PAGE_UP))
	//{
	//	velocity.y += 1;
	//}
	//// World-Based Downwards movement
	//if (glfwGetKey(Engine::windowPtr, GLFW_KEY_Q)
	//	|| glfwGetKey(Engine::windowPtr, GLFW_KEY_PAGE_DOWN))
	//{
	//	velocity.y -= 1;
	//}
	//#pragma endregion

	rigidbody->applyForce(Engine::timer.getDT() * force);	// Update the location of camera based on time
}


void MovementScript::jump()
{
	//if (glfwGetKey(Engine::windowPtr, GLFW_KEY_SPACE))
	//{
	//	jumpPresCur = true;
	//	if (jumpPresCur == true && jumpPresPrev == false)
	//		rigidbody->velocity.y = Rigidbody::jumpVel;
	//}

	//// On press of space bar, jump
	//if (rigidbody->velocity.y = 0.f
	//	&& glfwGetKey(Engine::windowPtr, GLFW_KEY_SPACE))
	//{
	//	rigidbody->velocity.y = Rigidbody::jumpVel;
	//}
}