#version 330 core

out vec4 ex_color;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;
in float visibility;

uniform sampler2D screenTexture; //Diffuse Texture
uniform sampler2D normalTexture; //Normal Mapping Texture
uniform bool normalMapping; //Enable or Disable Normal Mapping
uniform vec3 skyColour;

const float shineDamper = 32.0f;
const float reflectivity = 0.2;

void main()
{
	vec3 normal = texture(normalTexture, fs_in.TexCoords).rgb;
	if(normalMapping){
		normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
	}
    vec3 color = texture(screenTexture, fs_in.TexCoords).rgb;
    
	//Lighting
	vec3 ambient = 0.1 * color;
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shineDamper);
    vec3 specular = vec3(reflectivity) * spec;
    //

    ex_color = vec4(ambient + diffuse + specular, 1.0f);
	ex_color = mix(vec4(skyColour, 1.0), ex_color, visibility);
}