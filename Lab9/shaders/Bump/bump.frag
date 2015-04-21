//fragment shader, create a file for it named fragment.frag
#version 120

// TODO Step 0: Pass in 3 uniform variables
// Make sure they are named exactly as they are in the cpp file
// images are of type 'uniform sampler2D' and then the texture name.

uniform sampler2D brick_image;
uniform sampler2D bump_image;
uniform vec3 lightVector;

void main()
{
	gl_FragColor = vec4 (1.0, 0.0, 0.0, 0.0);

	// TODO Step 1:
	// Extract normals from the normal map

	// TODO Step 2:
	// Multiply lighting times original texture color to figure out how much light we receive


	// TODO Step 3:
	// Find the final color

}






