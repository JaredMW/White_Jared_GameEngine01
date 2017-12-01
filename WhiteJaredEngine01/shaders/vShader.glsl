#version 430

// Jared White
// vShader.glsl


// Model Inputs
in vec3 position;			// Model location in world
in vec2 uv;					// Model UV
in vec3 norm;				// Model norm
uniform mat4 objWorldMat;	// Local-to-world Matrix

// Camera Input
uniform mat4 worldView;	// woldView = Projection Matrix * Look-At Matrix

// General Input
uniform float time;		// Current simulation time


// Shader colors
float red;
float green;
float blue;

// Output to Frag shader
out vec3 fragColor;		// Fragment color
out vec3 fragLoc;		// Fragment location
out vec2 fragUV;		// Fragment UV
out vec3 fragNormal;	// Fragment normal

void main()
{
	// Set the colors for the fragments
	red = (position.y + 1) / 2;
	blue = (position.x + 1) / 2;
	green = (sin(time + blue - red)) / 2;
	
//	float green = (sin(time) + 1) / 2;
	if (green < 1) { green = -green; }
	fragColor = vec3(red, green, blue);


	// Draw the verticies and transform them into the perspective of the projection matrix
	gl_Position = worldView * objWorldMat * vec4(position, 1);

	// The positions, UVs, and normals of the fragments should equate to the vertex
	fragLoc = mat3(objWorldMat) * position;
	fragUV = uv;
	fragNormal = transpose(inverse(mat3(objWorldMat))) * norm;
}