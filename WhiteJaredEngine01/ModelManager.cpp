// Jared White
// April 21, 2017
// ModelManager.cpp


#include "ModelManager.h"
#include "TextureManager.h"
#include "Engine.h"

#include <sstream>
#include <fstream>
#include <iostream>


// Create a model manager with the default "models/" model directory
ModelManager::ModelManager()
{
	modelDir = "models/";
}

// Create a model manager with a specified model directory to look in for models
ModelManager::ModelManager(const char* modelDirectory)
{
	modelDir = modelDirectory;
}


ModelManager::~ModelManager()
{
}


// Attempt to add a model file. Return a pointer to the model if successful.
// Ensure all meshes and textures aren't already used.
// Return nullptr if it cannot be added.
Model* ModelManager::addModel(std::string modelFilename, const char* textureFilename, std::string modelTag)
{
	for (unsigned int i = 0; i < models.size(); i++)
	{
		// If repeat model
		if (modelFilename == models[i]->filename)
		{
			// If repeat model and texture
			if (textureFilename == models[i]->modelTexture.filename)
			{
				/*std::cout << "ERROR: Duplicate models created: \""
					<< modelFilename << "\" and \"" << textureFilename << "\"";*/

				// Check to see if there's already a Model with this tag
				/*if (validateModelTag((std::string)modelTag))
				{

				}*/

				// Return the address of the already-existing model
				return models[i];
			}

			// If repeat model and different texture
			else
			{
				// Prevent re-reading in all the Mesh data by setting it to the Model
				// which already exists
				Model* model = new Model(
					modelFilename.c_str(),
					models[i]->modelMesh,
					Texture { textureFilename },
					modelTag
				);


				/*Model model {
					modelFilename.c_str(),
					textureFilename,
					modelTag
				}*/

				// Attempt to read 
				/*if (!readMeshData(model))
				{*/
					//return false;
				//}

				//Texture texture{ textureFilename };

				//model.modelTexture = Texture { textureFilename };

				// Check to see if there's already a Model with this tag, and if so, replace it.
				if (validateModelTag(model->tag))
				{
					std::cout << model->tag;
				}

				// Add the new Model and return a pointer to it
				models.push_back(model);
				return (models.back());
			}
		}
	}


	// If completely new model
	//Model model(modelFilename.c_str(), Mesh{}, Texture{ textureFilename }, modelTag);// = { modelFilename };
	Model* model = new Model(modelFilename.c_str(), textureFilename, modelTag);
	if (!readMeshData(model))
	{
		return nullptr;
	}
	model->modelTexture = Texture { textureFilename };

	// Check to see if there's already a Model with this tag, and if so, replace it.
	validateModelTag(model->tag);

	// Add the new Model and return a pointer to it
	models.push_back(model);
	return (models.back());
}


// Attempt to efficiently add a Model. Return nullptr if fails.
// If already exists, set it equal to the already-existing Model.
Model* ModelManager::addModel(Model* model)
{
	for (unsigned int i = 0; i < models.size(); i++)
	{
		// If repeat Mesh
		if (model->filename == models[i]->filename)
		{
			// If both repeat Mesh and Texture
			if (model->modelTexture.filename == models[i]->modelTexture.filename)
			{
				return models[i];
			}

			// If repeat Mesh and different Texture
			else
			{
				// Check to see if there's already a Model with this tag,
				// and if so, update the tag
				if (validateModelTag(model->tag))
				{
					std::cout << model->tag;
				}

				// Fill in Mesh data from other 
				model->modelMesh = models[i]->modelMesh;

				// Add the new Model and return a pointer to it
				models.push_back(model);
				return (models.back());
			}
		}
	}


	// If completely new model
	if (!readMeshData(model))
	{
		return nullptr;
	}

	// Check to see if there's already a Model with this tag, and if so, replace it.
	validateModelTag(model->tag);

	// Add the new Model and return a pointer to it
	models.push_back(model);
	return (models.back());
}


// Attempt to upload all model data.
// Return false if unsuccsessful.
bool ModelManager::bufferModels()
{
	for (unsigned int i = 0; i < models.size(); i++)
	{
		if (!bufferModel(models[i]/*.filename, models[i].modelTexture.filename*/))
		{
			return false;
		}
	}

	return true;
}

// Delete all Model data and pointers
void ModelManager::unloadModels()
{
	while (models.size() > 0)
	{
		delete models.back();
		models.pop_back();
	}
}


// Attempt to read the model file to fill out its mesh data.
// Return false if unsuccsessful.
bool ModelManager::readMeshData(Model* model)
{
	#pragma region Read Model Data
	// Declare 4 vector collections for the model data
	std::vector<glm::vec3> locations;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<VertIndex> vertIndicies;


	// GET: Vertex locations, UVs, normals, and faces
	// Attempt to open the .obj file "objFileName" to read its contents
	// (.obj files are not binary like most model formats are)
	std::ifstream objFile;
	objFile.open((std::string)modelDir + model->filename/*, std::ios::binary*/);

	// If successfully opened...
	if (objFile.is_open())
	{
		std::string line;			// Current line being read
		std::stringstream stream;	// String stream to parse data
		std::string label; 			// Label for determining type of data

		// Seek & get the length
		//objFile.seekg(0, std::ios::end);
		//int fileLength = (int)objFile.tellg();	// Length of .obj file

		//// Seek back to the beginning
		//objFile.seekg(0, std::ios::beg);

		//// Read in the contents of the .obj file
		//char* modelContents = new char[fileLength + 1];	// Contents of .obj file
		//objFile.read(modelContents, fileLength);
		//modelContents[fileLength] = 0;


		/*glm::vec3 locData;		//		POTENTIAL OPTIMIZATION:
		glm::vec2 uvData;			// Declare these variables out here to prevent
		glm::vec3 normalData;		// creation of potentially thousands of variables
		char slash;					// thru each iteration of the input loop
		VertIndex indexData[3];*/
		
		model->modelMesh.minExtent = glm::vec3(0, 0, 0);
		model->modelMesh.maxExtent = glm::vec3(0, 0, 0);

		// Loop through every line in the file
		while (getline(objFile, line))
		{
			// Enter the line into the stream & determine action
			// based on the line label
			stream = std::stringstream(line);
			stream >> label;

			// TEMPORARY IF - Keep until all lines are read at once
			if (line != "")
			{
				// Add a new Vertex Location
				if (label == "v")
				{
					glm::vec3 locData;
					stream >> locData.x >> locData.y >> locData.z;

					// Update the min/max XYZ values of the mesh
					if (locData.x < model->modelMesh.minExtent.x)
					{	// Min X
						model->modelMesh.minExtent.x = locData.x;
					}
					else if (locData.x > model->modelMesh.maxExtent.x)
					{	// Max X
						model->modelMesh.maxExtent.x = locData.x;
					}

					if (locData.y < model->modelMesh.minExtent.y)
					{	// Min Y
						model->modelMesh.minExtent.y = locData.y;
					}
					else if (locData.y > model->modelMesh.maxExtent.y)
					{	// Max Y
						model->modelMesh.maxExtent.y = locData.y;
					}

					if (locData.z < model->modelMesh.minExtent.z)
					{	// Min Z
						model->modelMesh.minExtent.z = locData.z;
					}
					else if (locData.z > model->modelMesh.maxExtent.z)
					{	// Max Z
						model->modelMesh.maxExtent.z = locData.z;
					}

					// Push back the location data
					locations.push_back(locData);
				}

				// Add a new Vertex UV
				else if (label == "vt")
				{
					glm::vec2 uvData;
					stream >> uvData.x >> uvData.y;
					uvs.push_back(uvData);
				}

				// Add a new Vertex Normal
				else if (label == "vn")
				{
					glm::vec3 normalData;
					stream >> normalData.x >> normalData.y >> normalData.z;
					normals.push_back(normalData);
				}

				// Add a new Face / Vertex Indicies
				else if (label == "f")
				{
					char slash;
					VertIndex indexData[3];

					// Add the format #/#/# 3x
					for (int i = 0; i < 3; i++)
					{
						stream >> indexData[i].locIndex >> slash
							>> indexData[i].uvIndex >> slash
							>> indexData[i].normIndex;

						indexData[i].locIndex -= 1;
						indexData[i].uvIndex -= 1;
						indexData[i].normIndex -= 1;

						vertIndicies.push_back(indexData[i]);
					}
				}
				else if (label == "s")
				{
				}
			}
		}

		// Close the file when done reading it
		objFile.close();
	}

	// If cannot open, then failed. Return false.
	else
	{
		std::cout << "ERROR: Cannot open .obj file to read data from: \""
			<< model->filename << "\"";
		return false;
	}
	#pragma endregion


	// Determine number of verticies in Mesh
	model->modelMesh.vertCount = vertIndicies.size();

	// Update the minimum/maximum XYZ and the size of the Mesh
	model->modelMesh.size = (model->modelMesh.maxExtent - model->modelMesh.minExtent) / 2.f;

	// Duplicate all of the vertices into a single Vertex buffer
	model->modelMesh.verticies = std::vector<Vertex>(model->modelMesh.vertCount);
	for (unsigned int i = 0; i < model->modelMesh.vertCount; i++)
	{
		model->modelMesh.verticies[i] = {
			locations[vertIndicies[i].locIndex],
			uvs[vertIndicies[i].uvIndex],
			normals[vertIndicies[i].normIndex] };
	}

	return true;
}


// Buffer/load a model
bool ModelManager::bufferModel(Model* model)
{
	#pragma region Generating a Buffer Object
	#pragma region STEP 1: Generating a Buffer (VBO)
	// Setup GLuint variables to hold the numbers identifying
	// the array and buffer that's being generated

	// Create the buffers internally (and give back the indicies)
	glGenVertexArrays(1, &(model->modelMesh.vertArr));
	glGenBuffers(1, &(model->modelMesh.vertBuf));
	#pragma endregion


	#pragma region STEP 2: Binding the buffer
	// Call glBindBuffer to control which array and buffer are active
	glBindVertexArray(model->modelMesh.vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, model->modelMesh.vertBuf);
	#pragma endregion


	#pragma region STEP 3: Uploading Vertex Data to the Buffer
	glBufferData(GL_ARRAY_BUFFER,						// Where to copy to
		sizeof(Vertex) * model->modelMesh.vertCount,	// # of bytes to copy
		&(model->modelMesh.verticies[0]),				// Where to copy from
		GL_STATIC_DRAW);								// How to draw the buffer
	#pragma endregion


	#pragma region STEP 4: Describing the Buffer Layout
	// Enable the Location attribute
	glEnableVertexAttribArray(Engine::shader.shaderPositionLoc);	// Attribute index
	
	// Setup the Location attribute
	glVertexAttribPointer(
		Engine::shader.shaderPositionLoc,	// Attribute index
		3,									// Number of components (x, y, z)
		GL_FLOAT,							// Type of data
		GL_FALSE,							// Should we normalize the data?
		sizeof(Vertex),						// Stride (Bytes per vertex)
		(void*)0);							// Offset to this attribute


	// Enable the UV attribute
	glEnableVertexAttribArray(Engine::shader.shaderUVLoc);
	
	// Setup the UV attribute
	glVertexAttribPointer(
		Engine::shader.shaderUVLoc,		// Attribute index
		2,								// Number of components (x, y)
		GL_FLOAT,						// Type of data
		GL_FALSE,						// Should we normalize the data?
		sizeof(Vertex),					// Stride (Bytes per vertex)
		(void*) sizeof(glm::vec3));		// Offset


	// Enable the Norm attribute
	glEnableVertexAttribArray(Engine::shader.shaderNormLoc);
	
	// Setup the Norm attribute
	glVertexAttribPointer(
		Engine::shader.shaderNormLoc,	// Attribute index
		3,								// Number of components (x, y, z)
		GL_FLOAT,						// Type of data
		GL_FALSE,						// Should we normalize the data?
		sizeof(Vertex),					// Stride (Bytes per vertex)
		(void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

	glBindVertexArray(Engine::shader.shaderPositionLoc);	// Unbind when finished editing
	#pragma endregion
	#pragma endregion

	// Attempt to upload/assign texture for this model
	//if (model->modelTexture.textureID == GLuint()
	//	&& !Engine::textureManager.loadTexture(&model->modelTexture))
	//		/*model->modelTexture.filename,
	//		model->modelTexture.textureID))*/
	//{
	//	return false;
	//}
	if (!Engine::textureManager.loadTexture(&model->modelTexture))
	{
		return false;
	}

	// Mesh upload succsessful
	return true;
}


// Return a model by its tag name
Model* ModelManager::getModel(std::string modelTag)
{
	for (unsigned int i = 0; i < models.size(); i++)
	{
		if (models[i]->tag == modelTag)
		{
			return (models[i]);
		}
	}
	return nullptr;
}


// Check to see if the model tag already exists.
// If it does, add a number to the end of the tag and return false.
// Otherwise, return true.
bool ModelManager::validateModelTag(std::string& modelTag)
{
	int tagCount = 0;
	for (unsigned int i = 0; i < models.size(); i++)
	{
		// For each instance of the tag, add 1 to the tag count
		if (modelTag == models[i]->tag)
		{
			tagCount++;
		}
	}

	// If tag already exists, add the count number to the end
	if (tagCount > 0)
	{
		modelTag += " (" + tagCount;
		modelTag += ")";
		return false;
	}

	return true;
}