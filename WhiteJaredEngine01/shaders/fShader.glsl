#version 430

// Jared White
// fShader.glsl


// Input
uniform vec3 cameraLoc;			// Location of camera in world
uniform vec3 lightLoc;			// Location of light in world
uniform vec3 lightColor;		// Light color


// Fragment locations & normal
in vec3 fragLoc;				// Location of fragment
in vec2 fragUV;					// UV of fragment
in vec3 fragNormal;				// Normal vector of fragment


// Color
in vec3 fragColor;									// Shader color
uniform vec3 objColor = vec3(1.0f, 1.0f, 1.0f);		// Object color
out vec4 color;										// Final draw color

// Texture
uniform sampler2D texImage;							// Texture image
vec4 texColor = texture(texImage, fragUV);			// Texture color


// Phong-Blinn lighting theory
vec3 ambience;
const float ambientStrength = 0.6f;
//const float ambientStrength = 0.9f;

vec3 diffuse;
const float diffuseStrength = 0.1f;

vec3 specular;
const float specularAlpha = 8.0f;	//16.0f
const float specularStrength = 0.9f;

vec3 brightness;	// Summation of ambience, diffuse, and specularity


// Declare unit vectors
vec3 L;		// Fragment to light
vec3 V;		// Fragment to camera
vec3 H;		// Half-way vector between them
vec3 N;		// Fragment normals


void main()
{
	// Normalize the fragment normals for interpolated vector values
	N = normalize(fragNormal);
	
	// Calculate unit vectors
	L = normalize(lightLoc - fragLoc);		// Fragment to light location
	V = normalize(cameraLoc - fragLoc);		// Fragment to camera location
	H = normalize(L + V);					// Half-way vector between them

	// Use the unit vectors to calculate light component scalars
	ambience = lightColor * ambientStrength;											// Ambience
	diffuse = lightColor * max(dot(L, N), 0) * diffuseStrength;							// Diffuse
	specular = lightColor * pow(max(dot(H, N), 0), specularAlpha) * specularStrength;	// Specularity

	// Summation of brightness
	brightness = ambience + diffuse + specular;


	// Get the Texture color
	//texColor = texture(texImage, fragUV);
	//vec3 objColor = vec3(1.0f, 1.0f, 1.0f);		// No shader color

	// Final Color
	//color = vec4(fragColor * brightness, 1.0f) * texColor;	// Shader color
	color = vec4(objColor * brightness, 1.0f) * texColor;	// No shader color
	//color = vec4(fragColor * brightness, 1.0f);
	//color = vec4(brightness, 1.0f) * texColor;
	//color = vec4(brightness, 1.0f);				// Grayscale
}