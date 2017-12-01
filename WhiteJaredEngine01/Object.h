// Jared White
// March 6, 2017
// WhiteJaredEngine01 Object.h


#pragma once

#include <string>
#include "Transform.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#include <glm/glm.hpp>


class Object
{
public:
	// DATA
	Transform* transform;	// This Object's Transform
	std::string tag;		// Tag to refer to this object by


	// CONSTRUCTORS

	Object();
	//Object(Transform& transform);
	Object(glm::vec3 location, glm::vec3 rotation, glm::vec3 scale, std::string tag = "Object");
	virtual ~Object();


	// METHODS

	virtual void update() = 0;
	//void renderer();
};