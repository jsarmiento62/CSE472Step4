#version 330

in vec3 cubeTexCoords;

out vec4 fColor;

uniform samplerCube skybox;

void main()
{
	fColor = textureCube(skybox, cubeTexCoords);
}