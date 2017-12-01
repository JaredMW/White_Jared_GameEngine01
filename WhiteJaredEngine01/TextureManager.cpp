// Jared White
// April 21, 2017
// TextureManager.cpp


#include "TextureManager.h"

#include <FreeImage.h>
#include <iostream>


// Create a texture manager with the default "textures/" texture directory
TextureManager::TextureManager()
{
	textureDir = "textures/";
}

// Create a texture manager with a specified texture directory
TextureManager::TextureManager(const char* textureDirectory)
{
	textureDir = textureDirectory;
}

TextureManager::~TextureManager()
{
}


// Attempt to upload texture data. Return false if unsuccessful.
bool TextureManager::load(Texture* texture)
{
	// Ensure the image has a name so it can be loaded.
	if (texture->filename == nullptr)
	{
		texture->textureID = -1;
		std::cout << "ERROR: Texture cannot be loaded without a filename";
		return false;
	}

	// Concat the engine's texture directory with the texture name
	std::string texFile = (std::string)textureDir + texture->filename;
	const char* texPath = texFile.c_str();


	// Use FreeImage to attempt to load the texture
	FIBITMAP* image = FreeImage_Load(
		FreeImage_GetFileType(texPath, 0),
		texPath);

	// Check to see if the load worked
	// If not, return false and print an error
	if (image == nullptr)
	{
		texture->textureID = -1;
		std::cout << "ERROR: Could not load texture at: \""
			<< texPath << "\"";
		return false;
	}

	// Use FreeImage to convert the original image to a 32bit bmp
	FIBITMAP* image32Bit =
		FreeImage_ConvertTo32Bits(image);

	// Unload the original image from memory
	FreeImage_Unload(image);


	// Generate and bind the new texture
	glGenTextures(1, &(texture->textureID));
	glBindTexture(GL_TEXTURE_2D, texture->textureID);


	//  CONCEPT CHECK:
	// When a texture is loaded or read, the textur in VRAM
	// that will be written to or read from is the one that
	// is bound to the GL_TEXTURE_2D binding point.


	// Upload texture bytes
	glTexImage2D(
		GL_TEXTURE_2D,							// Target
		0,										// Level
		GL_SRGB_ALPHA,							// Internal Format
		FreeImage_GetWidth(image32Bit),			// Width of image
		FreeImage_GetHeight(image32Bit),		// Height of image
		0,										// Border
		GL_BGRA,								// Format
		GL_UNSIGNED_BYTE,						// Type of data
		(void*)FreeImage_GetBits(image32Bit));	// Pixel address


	// Set min filter to linear instead of mipmap linear
	glTexParameteri(
		GL_TEXTURE_2D,			// Target
		GL_TEXTURE_MIN_FILTER,	// Parameter name
		GL_LINEAR);				// Parameter value

	// Unload the 32bit image from RAM (not VRAM!)
	FreeImage_Unload(image32Bit);

	// Unbind the texture after uploading has finished
	glBindTexture(GL_TEXTURE_2D, 0);

	// Texture successfully uploaded - return true
	return true;
}


// Load all of the models. Return true if successful.
//bool TextureManager::start()
//{
//	// If not already loaded
//	if (!loaded)
//	{
//		loaded = true;
//
//		// Attempt to load all textures
//		for (unsigned int i = 0; i < textures.size(); i++)
//		{
//			if (!load(textures[i]))
//			{
//				return false;
//			}
//		}
//		// All files loaded successfully
//		return true;
//	}
//}


// Attempt to add and load a texture by its filename.
// Set the ID of the loaded texture if succsessfully loaded.
// Otherwise, set the texID to -1 and return false.
bool TextureManager::loadTexture(Texture* texture)
{
	int texIndex = getTextureIndex(texture->filename);

	// If it already exists, set the existing ID & return true
	if (texIndex != -1)
	{
		texture->textureID = textures[texIndex]->textureID;
		return true;
	}

	// If it doesn't exist, attempt to add it to the end and load it
	else
	{
		// Attempt to load and add the texture
		if (load(texture))
		{
			//texID = texture->textureID;
			textures.push_back(texture);
			return true;
		}

		// If failed to load, unsuccessful. Return false.
		else
		{
			texture->textureID = -1;
			return false;
		}
	}
}


// Attempt to add and load a texture by its filename.
// Set the ID of the loaded texture if succsessfully loaded.
// Otherwise, set the texID to -1 and return false.
bool TextureManager::loadTexture(const char* filename, GLuint* texID)
{
	int texIndex = getTextureIndex(filename);

	// If it already exists, set the existing ID & return true
	if (texIndex != -1)
	{
		texID = &textures[texIndex]->textureID;
		return true;
	}

	// If it doesn't exist, attempt to add it to the end and load it
	else
	{
		Texture* texture = new Texture { filename };

		// Attempt to load and add the texture
		if (load(texture))
		{
			*texID = texture->textureID;
			textures.push_back(texture);
			return true;
		}

		// If failed to load, unsuccessful. Return false.
		else
		{
			*texID = -1;
			return false;
		}
	}



	//int texIndex = getTextureIndex(texture.filename);

	//// If it already exists, return the existing ID
	//if (texIndex != -1)
	//{
	//	texture = textures[texIndex];
	//	return true;
	//}
	//else
	//{
	//	textures.push_back(texture);
	//}

	// If it doesn't exist, attempt to add it to the end and load it
	//else
	//{
	//	// Load the texture, if it exists
	//	if (texture.load())
	//	{
	//		// Store the texture and put the ID # into texID
	//		texID = texture.getID();
	//		textures.push_back(texture);

	//		return true;
	//	}

	//	// If it can't load...
	//	else
	//	{
	//		// Return false & store index as -1 - failed to load
	//		texID = -1;
	//		return false;
	//	}
	//}
}


// Draw this texture on the next drawn model
void TextureManager::useTexture(GLuint texID)
{
	glBindTexture(GL_TEXTURE_2D, texID);
}


// Unload all textures
void TextureManager::unloadTextures()
{
	// Unload the textures
	while (textures.size() > 0)
	{
		unload(textures.back());
		textures.pop_back();
	}
}


// Unload a single texture
void TextureManager::unload(Texture* texture)
{
	glDeleteTextures(1, &texture->textureID);
}


// Check to see if this texture file name is already in use
// If it is, return the existing index of the file
// If it is not, return -1
int TextureManager::getTextureIndex(const char* filename)
{
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		if (textures[i]->filename == filename)
		{
			return i;
		}
	}

	return -1;
}