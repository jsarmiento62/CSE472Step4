#version 330
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;

out vec3 cubeTexCoords;

uniform mat4 mP;
uniform mat4 mV;

void main(){

   // Transform vertex  position into eye coordinates
	vec4 pos = mP*mV * vec4(vPosition, 1.0);
	gl_Position = pos.xyww;
	cubeTexCoords = vPosition;
}

