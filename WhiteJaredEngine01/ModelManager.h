// Jared White
// April 21, 2017
// ModelManager.h


#pragma once

#include <string>
#include <vector>

//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>

#include "TextureManager.h"
//#include "Shader.h"
//#include "Model.h"


// A 3D point in model-space
struct Vertex
{
	glm::vec3 location;
	glm::vec2 uv;
	glm::vec3 normal;
};

// The 3-dimensional data of the physical appearance of an object
struct Mesh
{
	std::string filename;
	GLuint vertArr;
	GLuint vertBuf;
	std::vector<Vertex> verticies;
	unsigned int vertCount;

	glm::vec3 minExtent;
	glm::vec3 maxExtent;
	glm::vec3 size;

	Mesh operator= (Mesh& mesh)
	{
		filename = mesh.filename;
		vertArr = mesh.vertArr;
		vertBuf = mesh.vertBuf;
		verticies = mesh.verticies;
		vertCount = mesh.vertCount;
		minExtent = mesh.minExtent;
		maxExtent = mesh.maxExtent;

		return *this;
	}
};


//struct Material
//{
//	Shader* shader;
//	Texture* texture;
//	/*Texture* specularMap;
//	Texture* bumpMap;*/
//};


// Represents a 3-dimensional object with a Mesh and a Texture.
struct Model
{
	std::string filename;			// Name of the .obj file to look in
	Mesh modelMesh;					// Mesh that makes up the model
	Texture modelTexture;			// Texture that goes on the model
	std::string tag;				// Name to refer to this model by

	//glm::vec3 modelColor = glm::vec3(1.f, 1.f, 1.f);


	Model(const char* modelFilename, Mesh modelMesh, Texture modelTexture, std::string tag)
	{
		filename = modelFilename;
		this->modelMesh = modelMesh;
		this->modelTexture = modelTexture;
		this->tag = tag;
	}

	Model(const char* modelFilename, const char* textureFilename, std::string tag)
	{
		filename = modelFilename;
		modelMesh.filename = modelFilename;
		modelTexture.filename = textureFilename;
		this->tag = tag;
	}


	// Attempt to draw this Model to the screen
	void render() const
	{
		// Use this model's texture, if it has one
		if (modelTexture.textureID != -1)
		{
			TextureManager::useTexture(modelTexture.textureID);
		}

		// If it doesn't have one, don't use it
		else
		{
			// Unbind the texture
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		//// Use the object's color
		//glUniform3f(Engine::shader.shaderObjColor,
		//	modelColor.r, modelColor.g, modelColor.b);

		// Activate the vertex array
		glBindVertexArray(modelMesh.vertArr);

		// Draw the vertex array
		glDrawArrays(GL_TRIANGLES, 0, modelMesh.vertCount);

		// Unbind the model after drawing it
		glBindVertexArray(0);
	}
};


// A manager that attempts to safely add and store models
class ModelManager
{
private:
	// DATA
	const char* modelDir;
	std::vector<Model*> models;

	// STRUCTS
	struct VertIndex {
		unsigned int locIndex;
		unsigned int uvIndex;
		unsigned int normIndex;
	};

	// METHODS
	bool readMeshData(Model* model);
	bool bufferModel(Model* model/*const char* modelFilename, const char* textureFilename*/);
	//void renderModel(Model model) const;
	bool validateModelTag(std::string& modelTag);

public:


	// CONSTRUCTORS
	ModelManager();
	ModelManager(const char* modelDirectory);
	~ModelManager();

	// METHODS

	// Add a Model to the ModelManager and return a pointer to it.
	// If it already exists, return the pointer to the already-existing model.
	Model* addModel(std::string modelFilename, const char* textureFilename, std::string modelTag = "Model");

	// Add a Model to the ModelManager and return a pointer to it.
	// If it already exists, return the pointer to the already-existing model.
	Model* addModel(Model* model);

	// Attempt to upload all Model data BEFORE starting the Engine
	bool bufferModels();

	// Unload/delete all Model data
	void unloadModels();
	//void renderModels() const;

	Model* getModel(std::string modelTag);
};

