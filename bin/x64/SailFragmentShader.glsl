#version 330 core

in vec4 ex_Color;
in VS_OUT {
	vec3 FragmentPosition;
	vec3 Normal;
	vec2 TextCoords;
} fs_in;

out vec4 out_Color;

uniform sampler2D diffuseTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(void)
{
    vec3 color = texture(diffuseTexture, fs_in.TextCoords).rgb;
    // ambient
    vec3 ambient = 0.05 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragmentPosition);
    vec3 normal = normalize(fs_in.Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragmentPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.3) * spec; // assuming bright white light color
    ex_Color = vec4(ambient + diffuse + specular, 1.0);
}