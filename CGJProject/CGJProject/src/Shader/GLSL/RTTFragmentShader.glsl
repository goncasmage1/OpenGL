#version 330 core

in vec2 ex_textCoord;
out vec4 color;

uniform sampler2D screenTexture;

float near = 0.1;
float far = 300.0;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // back to NDC 
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
	color = texture(screenTexture, ex_textCoord);
	/*float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
	color = vec4(vec3(depth), 1.0);*/
}