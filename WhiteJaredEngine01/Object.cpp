// Jared White
// March 6, 2017
// WhiteJaredEngine01 Object.cpp


#include "Object.h"
#include <iostream>


//Create a default object with a default "Object" tag and transform
Object::Object()
{
	tag = "Object";
	transform = &Transform();
}

//Object::Object(Transform& transform)
//{
//}

// Create a default object with a specified transform
Object::Object(glm::vec3 location, glm::vec3 rotation, glm::vec3 scale, std::string tag)
	//: Object(Transform(location, rotation, scale))
{
	this->tag = tag;
	transform = &Transform(location, rotation, scale);
}


Object::~Object()
{
}


//void Object::renderer()
//{
//	std::cout << "Rendering " << tag << std::endl;
//}