//// Jared White
//// April 17, 2017
//// WhiteJaredEngine01 Texture.cpp
//
//
////		IMPORTANT NOTE: 
//// This file is currently organized in a slightly haphazard manner,
//// with many likely unnecessary function calls and checking. This is
//// because this Texture class is set up in such a way that it should
//// be in a "transitioning" phase, where it can be made into a
//// TextureManager class instead, with a Texture struct. This should
//// allow for more dynamic asset creation and loading in the final
//// engine product, especially if I can convert similar classes such
//// as the Model, Shader, and maybe even Camera classes.
//
//
//#include "Texture.h"
//#include "Engine.h"
//
//#include <FreeImage.h>
//#include <iostream>
//
//
//// Create a default Texture object.
//Texture::Texture()
//{
//	textureID = -1;
//}
//
//
//// Create a Texture from a specified file
//// (texID should only be specified if the texture exists already)
//Texture::Texture(char* texFileName/*, GLuint texID = -1*/)
//{
//	textureName = texFileName;
//
//	// Check to see if the texture already exists.
//	// If so, use it.
//
//	/*if (texID != -1)
//	{*/
//	GLuint texID = Engine::getTextureID(texFileName);
//		/*if (texID != -1)
//		{*/
//	Engine::addTexture(*this, texID);
//		//}
//	//}
//
//
//}
//
//
//Texture::~Texture()
//{
//}
//
//
//// Upload the texture data to the graphics card.
//// Return true if sucsessful.
//bool Texture::load()
//{
//	// Ensure the image has a name so it can be loaded.
//	if (textureName == nullptr)
//	{
//		textureID = -1;
//		std::cout << "ERROR: Texture cannot be loaded without a filename";
//		return false;
//	}
//
//	// Concat the engine's texture directory with the texture name
//	std::string texFile = (std::string)Engine::textureDir + textureName;
//	const char* texPath = texFile.c_str();
//
//
//	// Use FreeImage to attempt to load the texture
//	FIBITMAP* image = FreeImage_Load(
//		FreeImage_GetFileType(texPath, 0),
//		texPath);
//
//	// Check to see if the load worked
//	// If not, return false and print an error
//	if (image == nullptr)
//	{
//		std::cout << "ERROR: Could not load texture at: \""
//			<< texPath << "\"";
//		return false;
//	}
//
//	// Use FreeImage to convert the original image to a 32bit bmp
//	FIBITMAP* image32Bit =
//		FreeImage_ConvertTo32Bits(image);
//
//	// Unload the original image from memory
//	FreeImage_Unload(image);
//
//
//	// Generate and bind the new texture
//	glGenTextures(1, &textureID);
//	glBindTexture(GL_TEXTURE_2D, textureID);
//
//
//	//  CONCEPT CHECK:
//	// When a texture is loaded or read, the textur in VRAM
//	// that will be written to or read from is the one that
//	// is bound to the GL_TEXTURE_2D binding point.
//
//
//	// Upload texture bytes
//	glTexImage2D(
//		GL_TEXTURE_2D,							// Target
//		0,										// Level
//		GL_SRGB_ALPHA,							// Internal Format
//		FreeImage_GetWidth(image32Bit),			// Width of image
//		FreeImage_GetHeight(image32Bit),		// Height of image
//		0,										// Border
//		GL_BGRA,								// Format
//		GL_UNSIGNED_BYTE,						// Type of data
//		(void*)FreeImage_GetBits(image32Bit));	// Pixel address
//
//
//	// Set min filter to linear instead of mipmap linear
//	glTexParameteri(
//		GL_TEXTURE_2D,			// Target
//		GL_TEXTURE_MIN_FILTER,	// Parameter name
//		GL_LINEAR);				// Parameter value
//
//	// Unload the 32bit image from RAM (not VRAM!)
//	FreeImage_Unload(image32Bit);
//
//	// Unbind the texture after uploading has finished
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	// Texture successfully uploaded - return true
//	return true;
//}
//
//
//// Bind the texture to draw on the next drawn model
//void Texture::use()
//{
//	glBindTexture(GL_TEXTURE_2D, textureID);
//}
//
//// Bind any specified loaded texture for use
//void Texture::useTexture(const GLuint & texID)
//{
//	glBindTexture(GL_TEXTURE_2D, texID);
//}
//
//
//// Safely unload the texture data
//void Texture::unload()
//{
//	glDeleteTextures(1, &textureID);
//}
//
//
//// Get the name of this texture
//char* Texture::getFilename() const
//{
//	return textureName;
//}
//
//// Get the ID location of this texture
//GLuint Texture::getID() const
//{
//	return textureID;
//}
