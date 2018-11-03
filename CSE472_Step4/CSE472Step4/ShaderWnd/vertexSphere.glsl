#version 330
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;

out vec3 fN;
out vec3 fE;
out vec3 fL;
out vec2 texCoord;

uniform mat4 mPVM;
uniform mat4 mVM;

void main(){
   vec4 LightPosition = vec4(-5.f, 5.f, -5.f, 0.f);

   // Transform vertex  position into eye coordinates
   vec3 pos = (mVM*vec4(vPosition, 1.0)).xyz;

   // Transform vertex normal into eye coordinates
   fN = (mVM*vec4(vNormal, 0.0)).xyz;
   vec3 t1 = normalize(cross(vec3(0., 0., 1.), vNormal));
   vec3 t2 = cross(vNormal, t1);
   t1 = (mVM*vec4(t1, 0.0)).xyz;
   t2 = (mVM*vec4(t2, 0.0)).xyz;
        
   fE = vec3(dot(-pos, t1), dot(-pos, t2), dot(-pos,fN));
   fL = LightPosition.xyz - pos;
   fL = vec3(dot(fL, t1), dot(fL, t2), dot(fL, fN));  
   
   texCoord = vTexCoord;
	gl_Position = mPVM * vec4(vPosition, 1.0);
}

