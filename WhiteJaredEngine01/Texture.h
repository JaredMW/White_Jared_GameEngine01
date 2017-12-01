//// Jared White
//// April 17, 2017
//// WhiteJaredEngine01 Texture.h
//
//
//#include <GL\glew.h>
//#include <GLFW\glfw3.h>
//#include <glm\glm.hpp>
//
//
//#pragma once
//class Texture
//{
//private:
//	// DATA
//	GLuint textureID;	// Index of texture in memory
//	char* textureName;	// Name of texture being used
//
//public:
//	// DATA
//
//	// CONSTRUCTORS
//	Texture();
//	Texture(char* texFileName/*, GLuint texID = -1*/);
//	~Texture();
//
//	// METHODS
//	bool load();
//	void use();
//	void unload();
//	
//	char* getFilename() const;
//	GLuint getID() const;
//
//	static void useTexture(const GLuint& texID);
//};