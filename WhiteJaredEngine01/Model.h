//// Jared White
//// May 7, 2017
//// WhiteJaredEngine01 Model.h
//
//#pragma once
//
//
//// Represents a 3-dimensional object with a Mesh and a Texture.
//struct Model
//{
//public:
//	std::string filename;			// Name of the .obj file to look in
//	Mesh modelMesh;					// Mesh that makes up the model
//	Texture modelTexture;			// Texture that goes on the model
//	std::string tag;				// Name to refer to this model by
//
//
//	/*Model(const char* modelFilename, Mesh modelMesh, Texture modelTexture, std::string tag)
//	{
//		filename = modelFilename;
//		this->modelMesh = modelMesh;
//		this->modelTexture = modelTexture;
//		this->tag = tag;
//	}*/
//
//	/*Model(const char* modelFilename, const char* textureFilename, std::string tag)
//	{
//		filename = modelFilename;
//		modelMesh.filename = modelFilename;
//		modelTexture.filename = textureFilename;
//		this->tag = tag;
//	}*/
//
//	// Attempt to draw this Model to the screen
//	void render() const
//	{
//		// Use this model's texture, if it has one
//		if (modelTexture.textureID != -1)
//		{
//			TextureManager::useTexture(modelTexture.textureID);
//		}
//
//		// If it doesn't have one, don't use it
//		else
//		{
//			// Unbind the texture
//			glBindTexture(GL_TEXTURE_2D, 0);
//		}
//
//		// Activate the vertex array
//		glBindVertexArray(modelMesh.vertArr);
//
//		// Draw the vertex array
//		glDrawArrays(GL_TRIANGLES, 0, modelMesh.vertCount);
//
//		// Unbind the model after drawing it
//		glBindVertexArray(0);
//	}
//};
//
