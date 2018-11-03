#version 330
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;

out vec3 fN;
out vec3 fE;
out vec3 fL;
out vec2 texCoord;

uniform vec4 LightPosition;
uniform mat4 mPVM;
uniform mat4 mVM;

void main(){

   // Transform vertex  position into eye coordinates
   vec3 pos = (mVM*vec4(vPosition, 1.0)).xyz;

   // Transform vertex normal into eye coordinates
   fN = (mVM*vec4(vNormal, 0.0)).xyz;
        
   fE = -pos;
   fL = LightPosition.xyz - pos;
   
   texCoord = vTexCoord;
	gl_Position = mPVM * vec4(vPosition, 1.0);
}

