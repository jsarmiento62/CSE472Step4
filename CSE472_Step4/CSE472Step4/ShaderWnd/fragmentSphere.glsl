#version 330

in vec3 fN;
in vec3 fL;
in vec3 fE;
in vec2 texCoord;

out vec4 fColor;

uniform mat4 mVM;

uniform sampler2D bump_map;

void main()
{
   vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
   vec4 LightPosition;
   float Shininess = 100.;

   AmbientProduct = vec4(0.2, 0.2, 0.2, 1.);
   DiffuseProduct = vec4(0.4, 0.12, 0.15, 1.);
   SpecularProduct = vec4(.2, .2, .2, 1.);

   vec3 N = normalize( 2.*texture2D(bump_map, texCoord).xyz-1.0);
   vec3 E = normalize (fE);
   vec3 L = normalize (fL);

   vec3 H = normalize(L + E);

	// Compute terms in the illumination equation
    vec4 ambient = AmbientProduct;
    float Kd = max(dot(L, N), 0.0);
    vec4  diffuse = Kd*DiffuseProduct;
    float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4  specular = Ks * SpecularProduct;
    if(dot(L, N) < 0.0) 
        specular = vec4(0.0, 0.0, 0.0, 1.0);

    fColor = clamp(ambient + diffuse + specular, 0., 1.);
    fColor.a = DiffuseProduct.a;
}