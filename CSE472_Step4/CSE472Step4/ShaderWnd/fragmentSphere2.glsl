#version 330

in vec3 fN;
in vec3 fE;

out vec4 fColor;

uniform mat4 mVM;
uniform mat4 mV;

uniform samplerCube env_map;

void main()
{
   vec3 N = normalize (fN);
   vec3 E = normalize (fE);
   vec3 R = (inverse(mVM)*vec4(reflect(-E,N),0.)).xyz;

   fColor = texture(env_map, R);
}