// Jared White
// March 6, 2017
// WhiteJaredEngine01 Engine.cpp


// Inclusion & Using Statements
// "" implies I wrote it
// <> implies I didn't write it & I'm obtaining it from a directory
//#include <vector>
#include <iostream>

#include "Engine.h"
#include "MovementScript.h"
#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>


// Declare static data
// Pointer to engine window
GLFWwindow* Engine::windowPtr;
Timer Engine::timer;

Camera Engine::camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
Shader Engine::shader;


TextureManager Engine::textureManager("textures/");
ModelManager Engine::modelManager("models/");
std::vector<GameObject*> Engine::gameObjects;

char* Engine::shaderDir = "shaders/";
char* Engine::modelDir = "models/";
char* Engine::textureDir = "textures/";

std::vector<Light> Engine::lights;

int Engine::width = 1600;
int Engine::height = 1200;
int Engine::midWidth = (int)floorf(Engine::width * .5f);
int Engine::midHeight = (int)floorf(Engine::height * .5f);


Model* Engine::model1 = &Model("sphere.obj", "TestTexture.png", "SphereModel");
Model* Engine::model2 = &Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel");

bool Engine::gameOver = false;


bool Engine::start()
{
	if (glfwInit() == GL_FALSE) {
		return false;	// Proves the .lib and .dll files are in place
	}


	// Create a windowed window
	windowPtr =
		glfwCreateWindow(width, height, "Jared White - DSA1 Engine", NULL, NULL);

	glfwGetWindowSize(windowPtr, &width, &height);
	midWidth = (int)floorf(Engine::width * .5f);
	midHeight = (int)floorf(Engine::height * .5f);


	// Make the created window the current active window
	// Else, quit
	if (windowPtr != nullptr) {
		glfwMakeContextCurrent(windowPtr);
	}
	else {
		glfwTerminate();
		return false;
	}


	// Initialize GLEW
	// Else, quit
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return false;
	}


	// Seed the random
	srand((unsigned int)time(0));


	// Load game assets

	#pragma region
	// ====================================================
	//                      SHADERS:
	// ====================================================

	// Clear the screen color
	glClearColor(0.392f, 0.584f, 0.929f, 1.f);	// Cornflower blue
	glClearColor(.2f, .2f, .2f, 1.f);

	// Load and use the default shader
	if (!shader.load())
	{
		return false;
	}
	shader.use();

	// Enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable backface culling
	glEnable(GL_CULL_FACE);

	// Enable depth buffering
	glEnable(GL_DEPTH_TEST);
	#pragma endregion


	#pragma region Textures
	// ====================================================
	//                     TEXTURES:
	// ====================================================

	textureManager;
	/*textures.push_back(Texture("TestTexture.png"));
	loadTextures();*/
	#pragma endregion


	#pragma region Models and GameObjects
	// ====================================================
	//                MODELS / GAMEOBJECTS:
	// ====================================================

	// Import and buffer all Models and Texture, and setup GameObjects
	/*if (modelManager.addModel("sphere.obj", "TestTexture.png", "Sphere") == nullptr)
	{
		return false;
	}
	if (modelManager.addModel("quad.obj", "TestTexture.png", "Square") == nullptr)
	{
		return false;
	}
	if (modelManager.addModel("teapot.obj", "whitJ_P03c_texture.tga", "Tea") == nullptr)
	{
		return false;
	}
	if (modelManager.addModel("whitJ_Well.obj", "whitJ_P03c_texture.tga", "Well") == nullptr)
	{
		return false;
	}*/
	/*Model* model1 = new Model("sphere.obj", "TestTexture.png", "SphereModel");
	Model* model2 = new Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel");*/
	/*model1 = new Model("sphere.obj", "TestTexture.png", "SphereModel");
	model2 = new Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel");*/


	// Initialize Camera values
	camera = Camera(
		new Transform(
			glm::vec3(0, 0, 2),
			glm::vec3(0, 0, 0),
			glm::vec3(1, 1, 1)));


	// TEMPORARY - Instantly create new GameObjects & load their textures + models

	// Obj 0
	GameObject* sphere = new GameObject(
		new Model("sphere.obj", "snow.png", "SphereModel"),
		new Transform(glm::vec3(-4, -5, 1),
			glm::vec3(0, (float)M_PI / 2.f, 0),
			glm::vec3(.05f, .05f, .05f)),
		new Rigidbody(.1f),
		"Sphere");

	sphere->collider = Collider::sphere;// Spherical collider
	sphere->renderObject = false;


	// Obj 1
	GameObject* orb = new GameObject(
		new	Model("sphere.obj", "snowman.png", "SphereModel"),
		new Transform(glm::vec3(0, -1.f, 0),
			glm::vec3(0, 0, 0),
			glm::vec3(.2f, .2f, .2f)),
		new Rigidbody(.05f),
		"Orb");

	orb->collider = Collider::sphere;	// Spherical collider


	// Obj 2
	GameObject* player = new GameObject(
		new Model("sphere.obj", "snow.png", "SphereModel"),
		new Transform(glm::vec3(5.f, .5f, 2),
			glm::vec3(0, 0, 0),
			glm::vec3(.5f, .5f, .5f)),
		new Rigidbody(.8f),
		"Player");

	player->collider = Collider::sphere;		// Square AABB collider
	player->addScript(new MovementScript(&camera, 4, player->rigidbody));


	// Obj 3
	GameObject* floor = new GameObject(
		new Model("floor_pyramid.obj", "ice.png", "FloorModel"),
		new Transform(glm::vec3(0.f, -1.75f, 0.f),
			glm::vec3(0, 0, 0),
			glm::vec3(20.f, .25f, 20.f)),
		new Rigidbody(.1f),
		"Floor");

	floor->collider = Collider::aabb;	// Square AABB collider


	// Obj 4
	GameObject* well = new GameObject(
		new Model("well.obj", "whitJ_P03c_texture.tga", "WellModel"),
		new Transform(glm::vec3(3.f, 1.75f, -7.f),
			glm::vec3(0, 0, 0),
			glm::vec3(3.f, 3.f, 3.f)),
		new Rigidbody(.1f),
		"Well");

	well->collider = Collider::aabb;	// Square AABB collider


	// Obj 5
	GameObject* orb2 = new GameObject(
		new	Model("sphere.obj", "snowman.png", "SphereModel"),//Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),	//*&Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),*/model2,
		new Transform(glm::vec3(-8.7f, -1.f, 0),
			glm::vec3(0, 0, 0/*(float)M_PI / 4.f, (float)M_PI / 6.f, (float)M_PI * 2.f / 3.f*/),
			glm::vec3(.3f, .3f, .3f)),
		new Rigidbody(.05f),
		"Orb");

	orb2->collider = Collider::sphere;	// Spherical collider


	// Obj 6
	GameObject* orb3 = new GameObject(
		new	Model("sphere.obj", "snowman.png", "SphereModel"),//Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),	//*&Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),*/model2,
		new Transform(glm::vec3(-17.f, -1.f, -6.76f),
			glm::vec3(0, 0, 0/*(float)M_PI / 4.f, (float)M_PI / 6.f, (float)M_PI * 2.f / 3.f*/),
			glm::vec3(.4f, .4f, .4f)),
		new Rigidbody(.05f),
		"Orb");

	orb3->collider = Collider::sphere;	// Spherical collider


	// Obj 7
	GameObject* orb4 = new GameObject(
		new	Model("sphere.obj", "snowman.png", "SphereModel"),//Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),	//*&Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),*/model2,
		new Transform(glm::vec3(-5.f, -1.f, .4f),
			glm::vec3(0, 0, 0/*(float)M_PI / 4.f, (float)M_PI / 6.f, (float)M_PI * 2.f / 3.f*/),
			glm::vec3(.2f, .2f, .2f)),
		new Rigidbody(.05f),
		"Orb");

	orb4->collider = Collider::sphere;	// Spherical collider


	// Obj 8
	GameObject* orb5 = new GameObject(
		new	Model("sphere.obj", "snowman.png", "SphereModel"),//Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),	//*&Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),*/model2,
		new Transform(glm::vec3(13.f, -1.f, 6.f),
			glm::vec3(0, 0, 0/*(float)M_PI / 4.f, (float)M_PI / 6.f, (float)M_PI * 2.f / 3.f*/),
			glm::vec3(.4f, .4f, .4f)),
		new Rigidbody(.05f),
		"Orb");

	orb5->collider = Collider::sphere;	// Spherical collider


	// Obj 9
	GameObject* orb6 = new GameObject(
		new	Model("sphere.obj", "snowman.png", "SphereModel"),//Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),	//*&Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),*/model2,
		new Transform(glm::vec3(1.2f, -1.f, -7.f),
			glm::vec3(0, 0, 0/*(float)M_PI / 4.f, (float)M_PI / 6.f, (float)M_PI * 2.f / 3.f*/),
			glm::vec3(.3f, .3f, .3f)),
		new Rigidbody(.05f),
		"Orb");

	orb6->collider = Collider::sphere;	// Spherical collider


	// Obj 10
	GameObject* orb7 = new GameObject(
		new	Model("sphere.obj", "snowman.png", "SphereModel"),//Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),	//*&Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),*/model2,
		new Transform(glm::vec3(-15.f, -1.f, 3.5f),
			glm::vec3(0, 0, 0/*(float)M_PI / 4.f, (float)M_PI / 6.f, (float)M_PI * 2.f / 3.f*/),
			glm::vec3(.3f, .3f, .3f)),
		new Rigidbody(.05f),
		"Orb");

	orb7->collider = Collider::sphere;	// Spherical collider


	// Obj 11
	GameObject* orb8 = new GameObject(
		new	Model("sphere.obj", "snowman.png", "SphereModel"),//Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),	//*&Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),*/model2,
		new Transform(glm::vec3(10.f, -1.f, 4.f),
			glm::vec3(0, 0, 0/*(float)M_PI / 4.f, (float)M_PI / 6.f, (float)M_PI * 2.f / 3.f*/),
			glm::vec3(.4f, .4f, .4f)),
		new Rigidbody(.05f),
		"Orb");

	orb8->collider = Collider::sphere;	// Spherical collider


	// Obj 12
	GameObject* orb9 = new GameObject(
		new	Model("sphere.obj", "snowman.png", "SphereModel"),//Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),	//*&Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),*/model2,
		new Transform(glm::vec3(-15.f, -1.f, -14.6f),
			glm::vec3(0, 0, 0/*(float)M_PI / 4.f, (float)M_PI / 6.f, (float)M_PI * 2.f / 3.f*/),
			glm::vec3(.4f, .4f, .4f)),
		new Rigidbody(.05f),
		"Orb");

	orb9->collider = Collider::sphere;	// Spherical collider


	// Obj 13
	GameObject* orb10 = new GameObject(
		new	Model("sphere.obj", "snowman.png", "SphereModel"),//Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),	//*&Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),*/model2,
		new Transform(glm::vec3(-15.f, -1.f, 14.f),
			glm::vec3(0, 0, 0/*(float)M_PI / 4.f, (float)M_PI / 6.f, (float)M_PI * 2.f / 3.f*/),
			glm::vec3(.2f, .2f, .2f)),
		new Rigidbody(.05f),
		"Orb");

	orb10->collider = Collider::sphere;	// Spherical collider


	// Obj 14
	GameObject* orb11 = new GameObject(
		new	Model("sphere.obj", "snowman.png", "SphereModel"),//Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),	//*&Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),*/model2,
		new Transform(glm::vec3(18.f, -1.f, 15.f),
			glm::vec3(0, 0, 0/*(float)M_PI / 4.f, (float)M_PI / 6.f, (float)M_PI * 2.f / 3.f*/),
			glm::vec3(.4f, .4f, .4f)),
		new Rigidbody(.05f),
		"Orb");

	orb11->collider = Collider::sphere;	// Spherical collider


	// Obj 15
	GameObject* orb12 = new GameObject(
		new	Model("sphere.obj", "snowman.png", "SphereModel"),//Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),	//*&Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),*/model2,
		new Transform(glm::vec3(11.2f, -1.f, -16.f),
			glm::vec3(0, 0, 0/*(float)M_PI / 4.f, (float)M_PI / 6.f, (float)M_PI * 2.f / 3.f*/),
			glm::vec3(.3f, .3f, .3f)),
		new Rigidbody(.05f),
		"Orb");

	orb12->collider = Collider::sphere;	// Spherical collider


	// Obj 16
	GameObject* orb13 = new GameObject(
		new	Model("sphere.obj", "snowman.png", "SphereModel"),//Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),	//*&Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),*/model2,
		new Transform(glm::vec3(-16.f, -1.f, -17.5f),
			glm::vec3(0, 0, 0/*(float)M_PI / 4.f, (float)M_PI / 6.f, (float)M_PI * 2.f / 3.f*/),
			glm::vec3(.3f, .3f, .3f)),
		new Rigidbody(.05f),
		"Orb");

	orb13->collider = Collider::sphere;	// Spherical collider


	// Obj 17
	GameObject* orb14 = new GameObject(
		new	Model("sphere.obj", "snowman.png", "SphereModel"),//Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),	//*&Model("whitJ_Well.obj", "whitJ_P03c_texture.tga", "WellModel"),*/model2,
		new Transform(glm::vec3(11.f, -1.f, 18.f),
			glm::vec3(0, 0, 0/*(float)M_PI / 4.f, (float)M_PI / 6.f, (float)M_PI * 2.f / 3.f*/),
			glm::vec3(.4f, .4f, .4f)),
		new Rigidbody(.05f),
		"Orb");

	orb14->collider = Collider::sphere;	// Spherical collider


	// Attempt to buffer created models
	if (!modelManager.bufferModels())
	{
		return false;	// Buffering failed
	}

	// Enable wireframe rendering
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	#pragma endregion


	#pragma region Lighting
	// ====================================================
	//                      LIGHTING:
	// ====================================================

	// Create & store lights
	lights.push_back(
		Light {
			glm::vec3(0.f, 10.f, 10.f),	// Light location
			glm::vec3(1.f, 1.f, 1.f)	// Light color
		});

	// Upload the light information to the shader program
	glUniform3f(shader.shaderLightLoc,
		lights[0].location.x, lights[0].location.y, lights[0].location.z);
	glUniform3f(shader.shaderLightColor,
		lights[0].color.r, lights[0].color.g, lights[0].color.b);
	#pragma endregion


	#pragma region Camera
	// ====================================================
	//                       CAMERA:
	// ====================================================

	camera.start();
	camera.followDist = 2.5f;

	// Hide the mouse
	glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	#pragma endregion


	// Setup successful
	return true;
}


// Update the engine
void Engine::update()
{
	// Update the timer & clear the console
	timer.update();
	system("cls");
	std::cout << timer.getFPS();


	// Update the camera
	camera.update();

	// Update the GameObjects
	updateObjects();


	// Clear the canvas & depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update the shader
	glUniform1f(shader.shaderTimer, timer.getTime());



	/// For now, put all game logic here.
	/// The finished engine should allow game logic to be calculated in Main.cpp
	// =================================================
	//		TEMPORARY TEMPORARY TEMPORARY TEMPORARY
	// =================================================

	// Spin the snowman heads
	gameObjects[1]->transform->rotation.y += (float)3.f * timer.getDT() * gameObjects[1]->transform->scale.x;
	for (unsigned int i = 5; i < gameObjects.size(); i++)
	{
		gameObjects[i]->transform->rotation.y += (float)3.f * timer.getDT() * gameObjects[i]->transform->scale.x;
	}
	/*gameObjects[0]->rigidbody->applyForce(glm::vec3(.005f, .0011f, 0));
	gameObjects[1]->rigidbody->velocity.z = .4f;*/
	//gameObjects[3]->rigidbody->applyForce(glm::vec3(0, 0, .002f));
	//gameObjects[0]->transform->rotation.y += (float)0.523599f * timer.getDT();


	gameObjects[2]->rigidbody->applyGravity();
	// If player collides with floor...
	if (gameObjects[2]->isColliding(gameObjects[3]))
	{
		gameObjects[2]->rigidbody->acceleration.y = 0.f;
		gameObjects[2]->rigidbody->velocity.y = 0.f;
		gameObjects[2]->transform->location.y = gameObjects[2]->transform->scale.y + gameObjects[3]->transform->location.y + gameObjects[3]->transform->scale.y;
		//gameObjects[2]->rigidbody->applyForce(0.f, 9.81f * gameObjects[2]->rigidbody->gravityMultiplier, 0.f);
	}

	// If falling, then... fall.
	/*if (gameObjects[2]->transform->location.y > gameObjects[2]->transform->scale.y + gameObjects[3]->transform->location.y + gameObjects[3]->transform->scale.y + .001f)
	{*/
	//}


	// If player collides with well, teleport it to a random location
	if (gameObjects[2]->isColliding(gameObjects[4]))
	{
		gameObjects[4]->transform->location = glm::vec3((float)(rand() % (int)(gameObjects[3]->transform->scale.x * 2) - gameObjects[3]->transform->scale.x),
			gameObjects[4]->transform->location.y,
			(float)(rand() % (int)(gameObjects[3]->transform->scale.z * 2) - gameObjects[3]->transform->scale.z));
	}


	// If Player collides with any snowman heads, turn them invisible
	if (gameObjects[2]->isColliding(gameObjects[1])) { gameObjects[1]->renderObject = false; }
	for (unsigned int i = 5; i < gameObjects.size(); i++)
	{
		// Hide the object that's being collided with
		if (gameObjects[2]->isColliding(gameObjects[i]))
		{
			gameObjects[i]->renderObject = false;
		}
	}


	/*lights[0].location = gameObjects[2]->transform->location + glm::vec3(-3.f, -3.f, -3.f);
	glUniform3f(shader.shaderLightLoc,
		lights[0].location.x, lights[0].location.y, lights[0].location.z);*/

	camera.transform->location = gameObjects[2]->transform->location;

	// =================================================
	//		END END END END END END END END END END
	// =================================================


	//detectCollisions();
	if (gameObjects[2]->transform->location.y < -3.f)
	{
		gameOver = true;
	}

	// Draw all GameObjects to the screen
	renderObjects();


	// Swap the front and back buffers
	glfwSwapBuffers(windowPtr);

	// (If window lags, poll event processing)
	glfwPollEvents();
}


// Stop the Engine; terminate processes & unload data
void Engine::stop()
{
	// Unload the Shader
	shader.unload();

	// Unload the Textures
	textureManager.unloadTextures();

	// Unload the Models
	modelManager.unloadModels();

	// Unload the Lights
	/*for (unsigned int i = lights.size(); i > 0; i--)
	{
		lights.pop_back();
	}*/

	// Unload the GameObjects
	unloadGameObjects();
	/*for (unsigned int i = gameObjects.size(); i > 0; i--)
	{
		delete gameObjects[i-1]->objectModel;
		gameObjects.pop_back();
	}*/

	// Delete the Camera's Transform
	delete camera.transform;

	// End the program
	glfwTerminate();
}


// Add a game object to the Engine.
// Return true if successful.
bool Engine::addGameObject(GameObject* gameObject)
{
	if (gameObject->objectModel != nullptr)
	{
		/*if (modelManager.addModel(
			(*gameObject.objectModel).filename,
			(*gameObject.objectModel).modelTexture.filename,
			(*gameObject.objectModel).tag) != nullptr)
		{*/
		gameObjects.push_back(gameObject);
		return true;
		//}
	}

	return false;
}


// Check & Resolve all collisions
void Engine::detectCollisions()
{
	bool detected = false;
	for (unsigned int i = 0; i < gameObjects.size() - 1; i++)
	{
		for (unsigned int x = i + 1; x < gameObjects.size(); x++)
		{
			// If something is colliding with another thing
			if (gameObjects[i]->isColliding(gameObjects[x])) {
				// If nothing has yet been detected
				if (!detected)
				{
					// Turn the lights red if something is colliding
					lights[0].color = glm::vec3(1.f, 0, 0);
					glUniform3f(shader.shaderLightColor,
						lights[0].color.x, lights[0].color.y, lights[0].color.z);

					std::cout << "\nCOLLISION DETECTED";
					detected = true;

					//glClearColor(.75f, 0, 0, 1.f);
				}
			}

			// If nothing is colliding
			else
			{
				if (!detected)
				{
					// Keep lights white if nothing is colliding
					lights[0].color = glm::vec3(1.f, 1.f, 1.f);
					glUniform3f(shader.shaderLightColor,
						lights[0].color.x, lights[0].color.y, lights[0].color.z);

					glClearColor(.2f, .2f, .2f, 1.f);
				}
			}
		}
	}
}


// Update all of the GameObjects
void Engine::updateObjects()
{
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->update();
	}
}


// Render all of the GameObjects
void Engine::renderObjects()
{
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		// Draw it if it exists
		if (gameObjects[i]->objectModel != nullptr)
		{
			gameObjects[i]->render();
		}
	}
}


// Loop through and unload/delete all GameObjects
void Engine::unloadGameObjects()
{
	// Loop through each GameObject and delete them
	while (gameObjects.size() > 0)
	{
		//delete gameObjects[i]->objectModel;		// Delete model
		if (gameObjects.back()->rigidbody != nullptr) { delete gameObjects.back()->rigidbody; }	// Delete rigidbody
		delete gameObjects.back()->transform;		// Delete transform

		// Delete all Scripts
		while (gameObjects.back()->scripts.size() > 0)
		{
			delete gameObjects.back()->scripts.back();
			gameObjects.pop_back();
		}

		delete gameObjects.back();					// Delete the GameObject itself
		gameObjects.pop_back();
	}
}


// Add a new texture, and store the new index in texID after it's created.
// Or, store the index of the texture if it already exists.
// Returns false if fails.
//bool Engine::addTexture(Texture& texture, GLuint& texID)
//{
//	//GLuint newID = getTextureID(texture.getFileName());
//	int texIndex = getTextureIndex(texture.getFilename());
//
//	// If it already exists, return the existing ID
//	if (texIndex != -1)
//	{
//		texture = Engine::textures[texIndex];
//		texID = texture.getID();
//		return true/* newID*/;
//	}
//
//	// If it doesn't exist, attempt to add it to the end and load it
//	else
//	{
//		// Load the texture, if it exists
//		if (texture.load())
//		{
//			// Store the texture and put the ID # into texID
//			texID = texture.getID();
//			textures.push_back(texture);
//
//			return true;
//		}
//
//		// If it can't load...
//		else
//		{
//			// Return false & store index as -1 - failed to load
//			texID = -1;
//			return false;
//		}
//	}
//}


// Check to see if this texture file name is already in use
// If it is, return the existing ID of the file
// If it is not, return -1
//GLuint Engine::getTextureID(const char* filename)
//{
//	for (unsigned int i = 0; i < textures.size(); i++)
//	{
//		if (textures[i].getFilename() == filename)
//		{
//			return textures[i].getID();
//		}
//	}
//
//	return -1;
//}


// Check to see if this texture file name is already in use
// If it is, return the existing index of the file
// If it is not, return -1
//int Engine::getTextureIndex(const char* filename)
//{
//	for (unsigned int i = 0; i < textures.size(); i++)
//	{
//		if (textures[i].getFilename() == filename)
//		{
//			return i;
//		}
//	}
//
//	return -1;
//}


// Determine whether this texture file exists
//bool Engine::containsTexture(const char* filename)
//{
//	for (unsigned int i = 0; i < textures.size(); i++)
//	{
//		if (textures[i].getFilename() == filename)
//		{
//			return true;
//		}
//	}
//
//	return false;
//}

// Use a specified texture on the next model draw, if the specified ID is valid
//void Engine::useTexture(const GLuint &texID)
//{
//	textures[texID].use();
//}


// Attempt to load all textures. Return false if fails.
//bool Engine::loadTextures()
//{
//	//std::string textureName = "TestTexture.png";
//	for (unsigned int i = 0; i < textures.size(); i++)
//	{
//		// Attempt to load every created texture
//		if (!textures[i].load())
//		{
//			return false;
//		}
//	}
//
//	return true;
//}



// Buffer/load all models
// Return false if fails
//bool Engine::bufferModels()
//{
//	for (unsigned int i = 0; i < models.size(); i++)
//	{
//		if (!models[i].buffer(models[i].getModelName()))
//		{
//			std::cout << "ERROR - Cannot buffer model: \""
//				<< modelDir << models[i].getModelName() << "\"";
//			return false;
//		}
//	}
//
//	return true;
//}


// Draw all loaded models
//void Engine::renderModels()
//{
//	for (unsigned int i = 0; i < models.size(); i++)
//	{
//		models[i].render();
//	}
//}