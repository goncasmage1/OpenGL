#version 330 core

in vec4 in_Position;
in vec2 in_Coordinates;
in vec3 in_Normal;

uniform mat4 ModelMatrix;
uniform vec4 VertColor;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};


const float refractive_water = 1.33;
const float refractive_ice = 1.309;
const float refractive_glass = 1.52;
const float refractive_diamond = 2.42;

uniform vec3 cameraPos;

const float FresnelPower = 10;


out vec3 Refract;
out vec3 Reflect;
out vec3 I;
out vec3 Normal;

void main() 
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * in_Position;
	vec3 Normal = normalize(mat3(transpose(inverse(ModelMatrix))) * in_Normal);
	vec3 Position = vec3(ModelMatrix * in_Position);
	vec3 I = normalize(Position - cameraPos);


	float Eta = 1.0/ refractive_water;
	//float F  = ((1.0 - Eta) * (1.0 - Eta)) / ((1.0 + Eta) * (1.0 + Eta));
	//Ratio = F + (1.0 - F) * pow((1.0 - dot(-I, Normal)), FresnelPower);

	Refract = refract(I, Normal, Eta);
    //Refract = vec3(gl_TextureMatrix[0] * vec4(Refract, 1.0));
 
 
    Reflect = reflect(I, Normal);
    //Reflect = vec3(gl_TextureMatrix[0] * vec4(Reflect, 1.0));

}