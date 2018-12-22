#version 330 core

in vec4 clipSpace;
in vec3 toCameraVector;

out vec4 out_color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

const float indexDiamond = 2.41;
const float indexWater = 1.33;

float Fresnel(vec3 viewDir, vec3 Normal, float RefractiveIndex)
{
	//float R0 = 0.0204; //R0 
	//vec3 Normal = vec3(0.0f, 1.0f, 0.0f);
	float R0 = pow((1.0 - RefractiveIndex) / (1.0 + RefractiveIndex), 5);
	float cosTeta = dot(Normal, viewDir);
	
	return max(0, R0 + (1 - R0) * pow(1 - cosTeta, 5));
}

void main()
{
	vec2 ndc = (clipSpace.xy/clipSpace.w) / 2.0 + 0.5;
	vec2 refractTexCoord = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoord = vec2(ndc.x, -ndc.y);

	vec4 reflectionColour = texture(reflectionTexture, reflectTexCoord);	
	vec4 refractionColour = texture(refractionTexture, refractTexCoord);	
	
	//Fresnel
	vec3 viewVector = normalize(toCameraVector);
	//float refractiveFactor = dot(viewVector, vec3(0.0, 1.0, 0.0)); simples fresnel
	float refractiveFactor = Fresnel(viewVector, vec3(0.0f, 1.0f, 0.0f), indexDiamond);
	
	out_color = mix(refractionColour, reflectionColour, refractiveFactor);
}