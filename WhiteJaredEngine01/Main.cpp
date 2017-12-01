// Jared White
// March 6, 2017
// WhiteJaredEngine01 Main.cpp


//// Inclusion & Using Statements
//// "" implies I wrote it
//// <> implies I didn't write it & I'm obtaining it from a directory
//#include <vector>
//#include <iostream>
//#include <time.h>
//
//#include <FreeImage.h>
//
//#include <GL\glew.h>	// Order
//#include <GLFW\glfw3.h>	// matters!
//
//#include <glm\glm.hpp>
//#include <glm\gtx\transform.hpp>
//#include <glm\gtx\euler_angles.hpp>

#include "Engine.h"
#include <iostream>


int main() {
	// Attempt to successfully start the engine
	if (Engine::start())
	{
		//GameObject* ball = new GameObject(
		//	new Model("sphere.obj", "TestTexture.png", "SphereModel"),//*&Model("sphere.obj", "TestTexture.png", "SphereModel"),*/model1,
		//	glm::vec3(4, 0, 1),
		//	glm::vec3(0, 6.28f / 2.f, 0),
		//	glm::vec3(1, 3, 1),
		//	"Sphere");

		// MAIN GAME LOOP
		while (!glfwWindowShouldClose(Engine::windowPtr)
			&& !glfwGetKey(Engine::windowPtr, GLFW_KEY_ESCAPE)
			&& !Engine::gameOver)
		{
			Engine::update();


			
			// ===============
			// GAME LOGIC HERE
			// ===============

			//&obj.rigibody->applyForce(1.f, 0, 0,);

			// ===============
			// END  GAME LOGIC
			// ===============
		}

		// Safely stop & unload assets
		Engine::stop();
	}


	// If failure, hold console open for debugging
	else
	{
		std::cin.get();
	}
	
	return 0;
}