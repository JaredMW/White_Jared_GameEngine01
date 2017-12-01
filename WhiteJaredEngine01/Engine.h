// Jared White
// March 6, 2017
// WhiteJaredEngine01 Engine.h


#pragma once

#include "Timer.h"
#include "Shader.h"
#include "Camera.h"
#include "GameObject.h"
#include "ModelManager.h"
#include "TextureManager.h"

class Engine
{
	friend class ModelManager;
private:
	// DATA

	static std::vector<Light> lights;
	static std::vector<GameObject*> gameObjects;

	static Model* model1;
	static Model* model2;


	// METHODS
	
	// Update all gameObjects
	static void updateObjects();

	// Draw all gameObjects
	static void renderObjects();

	// Safely take out all GameObject data
	static void unloadGameObjects();


public:
	// DATA

	static GLFWwindow* windowPtr;
	static Timer timer;

	static Camera camera;
	static Shader shader;

	static char* shaderDir;
	static char* modelDir;
	static char* textureDir;

	static int width;
	static int height;

	static int midWidth;
	static int midHeight;

	static TextureManager textureManager;
	static ModelManager modelManager;

	static bool gameOver;



	// METHODS

	static bool start();
	static void stop();
	static void update();

	// Add a new GameObject to track
	static bool addGameObject(GameObject* gameObject);

	// Detect collisions
	static void detectCollisions();
};