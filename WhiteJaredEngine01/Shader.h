// Jared White
// March 22, 2017
// Shader.h


#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include <string>


// A simple Point Light
struct Light {
	glm::vec3 location;
	glm::vec3 color;
};

class Shader
{
private:
	// DATA

	GLuint programIndex, vIndex, fIndex;
	std::string filenameV;	// Filename of this Shader's vertex shader file
	std::string filenameF;	// Filename of this Shader's fragment shader file


	// METHODS

	bool compile(GLenum shaderType);
	void setUniformsAndAttribs();


public:
	// DATA

	GLuint shaderPositionLoc;
	GLuint shaderUVLoc;
	GLuint shaderNormLoc;
	GLuint shaderWorldViewMatLoc;
	GLuint shaderTimer;
	GLuint shaderCameraLoc;
	GLuint shaderLightLoc;
	GLuint shaderLightColor;
	GLuint shaderTextureLoc;
	GLuint shaderObjWorldMatLoc;
	GLuint shaderObjColor;


	// CONSTRUCTORS

	Shader();
	Shader(std::string vShaderName, std::string fShaderName);
	~Shader();


	// METHODS

	bool load();
	void use();
	void unload();
};