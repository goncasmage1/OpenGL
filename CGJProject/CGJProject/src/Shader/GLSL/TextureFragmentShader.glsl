#version 330 core

out vec4 color;

in vec2 ex_textCoord;

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} fs_in;

uniform sampler2D screenTexture; //Diffuse Texture
uniform sampler2D normalTexture; //Normal Mapping Texture

//uniform vec3 lightPos;	//DONE	
//uniform vec3 viewPos;	//FUCKED

uniform bool normalMapping; //Enable or Disable Normal Mapping	

uniform vec3 lightPosition;
uniform vec3 lightColour;

void main()
{
	//vec3 normal = normalize(fs_in.Normal);
	vec3 normal = texture(normalTexture, ex_textCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	//normal = normalize(fs_in.TBN * normal);

	vec3 Normalcolor = texture(screenTexture, ex_textCoord).rgb;

	vec3 ambient = 0.1 * Normalcolor;

	//vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * Normalcolor;

	//vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 specular = vec3(0.2) * spec;

	color = vec4(ambient + diffuse + specular, 1.0f);
	//color = texture(screenTexture, ex_textCoord);
}