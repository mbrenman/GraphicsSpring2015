//fragment shader, create a file for it named fragment.frag
#version 330

uniform vec3 lightVector;
uniform sampler2D brick_image;  // The brick texture
uniform int usingTexture;

out vec4 outputColor;

void main()
{
	

	vec4 final_color = vec4 (1.0, 0.0, 0.0, 1.0);


	outputColor = final_color;
}






