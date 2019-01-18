#version 330 core

in vec4 clipSpace;
in vec2 textureCoords;
in vec3 toCameraVector;
in vec3 fromLightVector;
in float visibility;

out vec4 out_color;


uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform float near;
uniform float far;
uniform float moveFactor;
uniform vec3 lightColour;
uniform vec3 skyColour;


//--------- CONSTANTS ----------------

const float indexDiamond = 2.41;
const float indexWater = 1.33;

const float waveStrenght = 0.04f;

const float shineDamper = 15.0f;
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
	
	//// Depth Calculation /////
	float depth = texture(depthMap, refractTexCoord).r;
	float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0)* (far - near));
	
	depth = gl_FragCoord.z;
	float waterDistance =  2.0 * near * far / (far + near - (2.0 * depth - 1.0)* (far - near));
	float waterDepth = floorDistance - waterDistance;

	//// Distortion ////
	vec2 distortedTextureCoords = texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg * 0.1;
	distortedTextureCoords = textureCoords + vec2(distortedTextureCoords.x, distortedTextureCoords.y + moveFactor);
	vec2 totalDistortion = texture(dudvMap, distortedTextureCoords.rg * 2.0 - 1.0).rg * waveStrenght * clamp(waterDepth / 5.0, 0.0, 1.0);

	refractTexCoord += totalDistortion;
	refractTexCoord = clamp(refractTexCoord, 0.001, 0.999); //fix white edges when the camera is in the water
	
	reflectTexCoord += totalDistortion;
	reflectTexCoord.x = clamp(reflectTexCoord.x, 0.001, 0.999); 
	reflectTexCoord.y = clamp(reflectTexCoord.y, -0.999, -0.001); 

	vec4 reflectionColour = texture(reflectionTexture, reflectTexCoord);	
	vec4 refractionColour = texture(refractionTexture, refractTexCoord);	
	/////////////

	vec4 normalMapColour = texture(normalMap, distortedTextureCoords);
	vec3 normal = vec3(normalMapColour.r * 2.0 - 1.0, normalMapColour.b * 3.0, normalMapColour.g* 2.0 - 1.0);
	normal = normalize(normal);
	

	vec3 viewVector = normalize(toCameraVector);
	vec3 lightVector = normalize(fromLightVector);

	//// Light ////
	//Ambient 
	float ambientStrenght = 0.1f;
	vec3 ambient = ambientStrenght * lightColour;

	//Diffuse
	float diff = max(dot(normal, lightVector), 0.0);
	vec3 diffuse = diff * lightColour;

	//Specular
	vec3 reflectedLight = reflect(lightVector, normal);
	float specular = max(0.0, dot(viewVector, reflectedLight));
	specular = pow(specular, shineDamper);
	vec3 specularHighlights = specular * reflectivity * lightColour * clamp(waterDepth, 0.0, 1.0);
	
	//vec3 color_result = (ambient + diffuse + specularHighlights) * waterColour;


	//// Fresnel ////
	float refractiveFactor = Fresnel(viewVector, normal, indexWater);
	
	////////////////////////////////////////////////////////
	out_color = mix(refractionColour, reflectionColour, refractiveFactor);
	out_color = mix(out_color, waterColour, 0.2) + vec4(specularHighlights, 0.0); //blue color
	out_color.a = clamp(waterDepth, 0.0, 1.0);
	out_color = mix(vec4(skyColour, 1.0), out_color, visibility);
}