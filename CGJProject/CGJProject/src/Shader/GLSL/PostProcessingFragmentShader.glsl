#version 330 core

in vec2 f_texcoord;
uniform sampler2D fbo_texture;
uniform int mode = 0;
uniform vec3 rgbIntensity = vec3(0.0, 0.0, 0.0);
uniform float offset = 0.0;
uniform float distortionAmount = 0.0;
out vec4 color;

void main(void) {
  vec2 texcoord = f_texcoord;
  float distortion = sin(texcoord.y * 4.0*2.0*3.14159 + offset);
  texcoord.x += (distortion * distortionAmount);
  switch(mode) {
    //Overlay
    case 0:
      color = texture(fbo_texture, texcoord) + (vec4(rgbIntensity, 1.0));
      break;
    //Filter
    case 1:
      color = texture(fbo_texture, texcoord) * (vec4(rgbIntensity, 1.0));
      break;
  }
}
