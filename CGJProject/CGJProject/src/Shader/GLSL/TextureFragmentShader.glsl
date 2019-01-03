#version 330 core

in vec2 ex_textCoord;
//in vec3 normal;
out vec4 color;

uniform sampler2D screenTexture;

uniform vec3 lightPosition;
uniform vec3 lightColour;

void main()
{
	//Ambient 
	/*float ambientStrenght = 0.1f;
	vec3 ambient = ambientStrenght * lightColour;

	//Diffuse
	float diff = max(dot(normal, fromLightVector), 0.0);
	vec3 diffuse = diff * lightColour;

	//Specular
	float specularStrenght = 0.9f;

	vec3 reflectDir = reflect(fromLightVector, normal);
	float spec = pow(max(dot(viewVector, reflectDir), 0.0), 32);
	vec3 specular = specularStrenght * spec * lightColour;

	//Sum of all components
	vec3 color_result = (ambient + diffuse + specular);*/
	
	color = texture(screenTexture, ex_textCoord); // +color_result;
}