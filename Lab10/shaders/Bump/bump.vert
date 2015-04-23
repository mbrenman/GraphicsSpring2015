#version 330 //Most current OpenGL version avaible in BGE at this shader creation time

uniform mat4 projectionMatrix, modelviewMatrix;
uniform ivec4 mouseScreenInfo;

in vec4 position;

void main()
{

	vec4 finalPosition = projectionMatrix * modelviewMatrix * position;


	gl_Position = finalPosition;
}



