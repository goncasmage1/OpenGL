#version 330 core

in vec4 clipSpace;
in vec2 textureCoords;
in vec3 toCameraVector;

out vec4 out_color;


uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform float moveFactor;
uniform vec3 lightColour;

uniform sampler2D depthMap;


//--------- CONSTANTS ----------------

const float indexDiamond = 2.41;
const float indexWater = 1.33;

const float waveStrenght = 0.01f;

const float near = 0.1;
const float far = 500.0;

const float shineDamper = 20.0f;
const float reflectivity = 0.6f;

const vec4 waterColour = vec4(0.0, 0.3, 0.4, 1.0);

//--------------------------------------
float Fresnel(vec3 viewDir, vec3 Normal, float RefractiveIndex)
{
	float R0 = pow((1.0 - RefractiveIndex) / (1.0 + RefractiveIndex), 5);
	float cosTeta = dot(Normal, viewDir);
	
	return max(0, R0 + (1 - R0) * pow(1 - cosTeta, 5));
}

void main()
{
	//// Normalized Device Space Coordinates ////
	vec2 ndc = (clipSpace.xy/clipSpace.w) / 2.0 + 0.5;
	vec2 refractTexCoord = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoord = vec2(ndc.x, -ndc.y);
	//////////////////////////////////////////////

	float depth = texture(depthMap, refractTexCoord).r;
	float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
	
	//depth = gl_FragCoord.z; //depth
	//float waterDistance =  2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
	//float waterDepth = floorDistance - waterDistance;

	//Distortion 
	vec2 distortedTextureCoords = texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg * 0.1;
	distortedTextureCoords = textureCoords + vec2(distortedTextureCoords.x, distortedTextureCoords.y + moveFactor);
	vec2 totalDistortion = texture(dudvMap, distortedTextureCoords.rg * 2.0 - 1.0).rg * waveStrenght;

	refractTexCoord += totalDistortion;
	refractTexCoord = clamp(refractTexCoord, 0.001, 0.999); //fix white edges when the camera is in the water
	
	reflectTexCoord += totalDistortion;
	reflectTexCoord.x = clamp(reflectTexCoord.x, 0.001, 0.999); 
	reflectTexCoord.y = clamp(reflectTexCoord.y, -0.999, -0.001); 

	vec4 reflectionColour = texture(reflectionTexture, reflectTexCoord);	
	vec4 refractionColour = texture(refractionTexture, refractTexCoord);	
	
	vec4 normalMapColour = texture(normalMap, distortedTextureCoords);
	vec3 normal = vec3(normalMapColour.r * 2.0 - 1.0, normalMapColour.b*5.0, normalMapColour* 2.0 - 1.0);
	normal = normalize(normal);
	

	
	//Light 
	////////////////////////////////////////////////////////
	//Ambient 
	//float ambientStrenght = 0.1f;
	//vec3 ambient = ambientStrenght * lightColour;

	//Diffuse
	//float diff = max(dot(normal, fromLightVector), 0.0);
	//vec3 diffuse = diff * lightColour;

	//Specular
	//float specularStrenght = 0.9f;
	
	//vec3 reflectDir = reflect(fromLightVector, normal);
	//float spec = pow(max(dot(viewVector, reflectDir), 0.0), 32);
	//vec3 specular = specularStrenght * spec * lightColour;

	//vec3 color_result = (ambient + diffuse + specular) * waterColour;

	////////////////////////////////////////////////////////

	/*vec3 fromLightVector = lightPos;
	vec3 reflectedLight = reflect(fromLightVector, normal);
	float specular = max(dot(reflectedLight, viewVector), 0.0);
	specular = pow(specular, shineDamper);
	vec3 specularHighlights =  specular * reflectivity * lightColour;

	*/
	vec3 viewVector = normalize(toCameraVector);

	//Fresnel
	float refractiveFactor = Fresnel(viewVector, vec3(0.0, 1.0, 0.0), indexWater);

	out_color = mix(refractionColour, reflectionColour, refractiveFactor);
	//out_color = mix(out_color, waterColour, 0.1) + vec4(specularHighlights, 0.0); //blue color

	//out_color = vec4(specularHighlights, 1.0);
}