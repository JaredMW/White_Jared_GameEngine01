// Jared White
// March 22, 2017
// WhiteJaredEngine01 Shader.cpp


#include "Shader.h"
#include "Engine.h"		// NOTE: Instead of needing this line, I could just include directory paths static strings in this file

#include <fstream>
#include <iostream>



Shader::Shader()
{
	filenameV = "vShader.glsl";
	filenameF = "fShader.glsl";
}

Shader::Shader(std::string vShaderName, std::string fShaderName)
{
	filenameV = vShaderName;
	filenameF = fShaderName;
}

Shader::~Shader()
{
}


// Load shaders.
// Will set the value of programs to 0 if it fails, or something other than 0
// if it succeeds.
bool Shader::load()
{
	// Compile the Vertex and Fragment shaders. If either are unsuccessful, return false.
	if (compile(GL_VERTEX_SHADER) == false
		|| compile(GL_FRAGMENT_SHADER) == false)
	{
		return false;
	}

	// Create a shader program
	programIndex = glCreateProgram();	// Index of a single, linked shader program

	// Attach the shaders to the shader program.
	glAttachShader(programIndex, vIndex);
	glAttachShader(programIndex, fIndex);

	// Link the program
	glLinkProgram(programIndex);

	// Confirm successful linking of the program
	GLint linkResult;
	glGetProgramiv(programIndex, GL_LINK_STATUS, &linkResult);

	// If linking failed, get & display the debug log, and return false
	if (linkResult == 0)
	{
		// Get the length of the link log
		GLint logLength;
		glGetProgramiv(programIndex, GL_INFO_LOG_LENGTH, &logLength);

		// Get the contents of the link log
		GLchar* logContents = new GLchar[logLength];
		glGetProgramInfoLog(programIndex, logLength, 0, logContents);

		// Display the log info
		std::cout << "ERROR: Linking error:"
			<< std::endl << logContents;

		// Delete allocated memory & created programs and shaders
		delete[] logContents;
		glDeleteProgram(programIndex);
		glDeleteShader(vIndex);
		glDeleteShader(fIndex);

		return false;
	}

	// If successful, return true
	else
	{
		// Setup the Uniforms and Attributes for this shader
		setUniformsAndAttribs();
		return true;
	}
}


// Use this shader program the next time something is drawn
void Shader::use()
{
	glUseProgram(programIndex);
}


// Calls glDeleteProgram(programIndex), glDeleteShader(vIndex), and glDeleteShader(fIndex)
// to deallocate this thader's memory from the graphics card. Set the 3 integers to 0 to
// indicate they are not on the graphics card.
void Shader::unload()
{
	glDeleteProgram(programIndex);
	glDeleteShader(vIndex);
	glDeleteShader(fIndex);

	programIndex = 0;
	vIndex = 0;
	fIndex = 0;

	filenameF = "";
	filenameV = "";

	shaderPositionLoc = 0;
	shaderUVLoc = 0;
	shaderNormLoc = 0; 
	shaderWorldViewMatLoc = 0;
	shaderTimer = 0;
	shaderCameraLoc = 0;
	shaderLightLoc = 0;
	shaderLightColor = 0;
	shaderTextureLoc = 0;
	shaderObjColor = 0;
}


// Helper method used by load to read and compile a shader file and save the index where
// it is stored.
bool Shader::compile(GLenum shaderType)
{
	// Open the vertex or fragment shader file.
	std::ifstream shaderFile;

	// If shaderType is GL_VERTEX_SHADER...
	if (shaderType == GL_VERTEX_SHADER)
	{
		shaderFile.open(Engine::shaderDir + filenameV, std::ios::binary);
	}

	// If shaderType is GL_FRAGMENT_SHADER...
	else if (shaderType == GL_FRAGMENT_SHADER)
	{
		shaderFile.open(Engine::shaderDir + filenameF, std::ios::binary);
	}

	else
	{
		std::cout << "ERROR: Invalid shader type. Cannot compile shader." << std::endl;
	}


	// If open, load its text into a null-terminated array of characters
	if (shaderFile.is_open())
	{
		// Seek & get the length
		shaderFile.seekg(0, std::ios::end);
		int fileLength = (int)shaderFile.tellg();	// Length of shader file

		// Seek back to the beginning
		shaderFile.seekg(0, std::ios::beg);

		// Read in the contents of the shader file
		char* shaderContents = new char[fileLength + 1];	// Contents of shader file
		shaderFile.read(shaderContents, fileLength);
		shaderContents[fileLength] = 0;

		// Close the shader file
		shaderFile.close();


		// Create, source, and compile a shader of the appropriate shader type
		GLint compResult;	// Result of compilation
		GLuint shaderIndex;	// Shader program index
		shaderIndex = glCreateShader(shaderType);

		if (shaderType == GL_VERTEX_SHADER)
		{
			vIndex = shaderIndex;
			glShaderSource(vIndex, 1, &shaderContents, 0);
			glCompileShader(vIndex);
			glGetShaderiv(vIndex, GL_COMPILE_STATUS, &compResult);
		}
		else if (shaderType == GL_FRAGMENT_SHADER)
		{
			fIndex = shaderIndex;
			glShaderSource(fIndex, 1, &shaderContents, 0);
			glCompileShader(fIndex);
			glGetShaderiv(fIndex, GL_COMPILE_STATUS, &compResult);
		}

		// Delete allocated memory for shader content
		delete[] shaderContents;

		// Determine if the shader compiled by calling glGetShaderiv()
		// If compilation successful, return true.
		// Else, display debug log w/ error information.
		if (compResult == 0)
		{
			// Get the length of the shader log
			GLint length;
			glGetShaderiv(shaderIndex, GL_INFO_LOG_LENGTH, &length);

			// Make a GLchar array
			GLchar* shaderLog = new GLchar[length];

			// Get the log info & store it in the array
			glGetShaderInfoLog(shaderIndex, length, 0, shaderLog);

			// Print the log information
			std::cout << "ERROR: Shader could not compile:"
				<< std::endl << shaderLog;

			// Delete the created shader program & allocated memory
			if (shaderType == GL_VERTEX_SHADER)
			{
				glDeleteShader(vIndex);
			}
			else if (shaderType == GL_FRAGMENT_SHADER)
			{
				glDeleteShader(fIndex);
			}

			delete[] shaderLog;
			return false;
		}

		// Successful compilation - return true
		else
		{
			return true;
		}
	}

	// If shader file could not be opened...
	else
	{
		std::cout << "ERROR: Could not open and read shader file." << std::endl;
		return false;
	}
}


// Set the uniform and attribute indicies for the shader
void Shader::setUniformsAndAttribs()
{
	shaderPositionLoc	  = glGetAttribLocation(programIndex, "position");
	shaderUVLoc			  = glGetAttribLocation(programIndex, "uv");
	shaderNormLoc		  = glGetAttribLocation(programIndex, "norm");
	shaderWorldViewMatLoc = glGetUniformLocation(programIndex, "worldView");
	shaderTimer			  = glGetUniformLocation(programIndex, "time");
	shaderCameraLoc		  = glGetUniformLocation(programIndex, "cameraLoc");
	shaderLightLoc		  = glGetUniformLocation(programIndex, "lightLoc");
	shaderLightColor	  = glGetUniformLocation(programIndex, "lightColor");
	shaderTextureLoc	  = glGetUniformLocation(programIndex, "texImage");
	shaderObjWorldMatLoc  = glGetUniformLocation(programIndex, "objWorldMat");
	shaderObjColor		  = glGetUniformLocation(programIndex, "objColor");
}
