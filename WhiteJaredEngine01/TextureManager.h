// Jared White
// April 21, 2017
// WhiteJaredEngine01 TextureManager.h


#include <string>
#include <vector>


#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>


#pragma once
struct Texture
{
	const char* filename;
	GLuint textureID;
};


class TextureManager
{
private:
	// DATA
	std::vector<Texture*> textures;
	bool loaded = false;


	// METHODS
	
	// Unload texture data
	void unload(Texture* texture);
	
	// Upload the data of the texture, and return result of success
	bool load(Texture* texture);

	// Get the index of a loaded texture by its filename
	int getTextureIndex(const char* filename);


public:
	// DATA 
	const char* textureDir;		// The directory to read texture files from


	// CONSTRUCTORS

	TextureManager();
	TextureManager(const char* textureDirectory);
	~TextureManager();


	// METHODS

	// Attempt to add and load a texture by its filename.
	// Set the ID of the loaded texture if succsessfully loaded.
	// Otherwise, set the texID to -1 and return false.
	bool loadTexture(const char* filename, GLuint* texID);

	// Attempt to add and load a texture by its filename.
	// Set the ID of the loaded texture if succsessfully loaded.
	// Otherwise, set the texID to -1 and return false.
	bool loadTexture(Texture* texture);

	// Draw this texture on the next drawn model
	static void useTexture(GLuint texID);

	// Safely unload texture data
	void unloadTextures();
};

