#version 330 core

out vec4 color;

in vec2 ex_textCoord;

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec4 LightVec;
	vec4 ViewVec;
} fs_in;

uniform sampler2D screenTexture; //Diffuse Texture
uniform sampler2D normalTexture; //Normal Mapping Texture

uniform bool normalMapping; //Enable or Disable Normal Mapping	

uniform vec3 lightPosition;
uniform vec3 lightColour;
in vec3 worldLight;
void main()
{
	vec3 normal = texture(normalTexture, ex_textCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	vec3 Normalcolor = texture(screenTexture, ex_textCoord).rgb;


	//Ambient Light
	vec3 ambient = 0.5* Normalcolor;

	//Diffuse Light
	vec4 lightDir = normalize(fs_in.LightVec);
	float diff = max(dot(vec3(lightDir), normal), 0.0);
	vec3 diffuse = diff * Normalcolor;

	//Specular Light
	vec4 viewDir = normalize(fs_in.ViewVec);
	vec3 reflectDir = reflect(vec3(-lightDir), normal);
	vec4 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, vec3(halfwayDir)), 0.0), 32.0);
	vec3 specular = vec3(0.2) * spec;
		
	
	color = vec4(ambient + diffuse + specular, 1.0f);

	color = texture(normalTexture, ex_textCoord);
	color = texture(screenTexture, ex_textCoord);
	color = vec4(specular, 1.0);
}