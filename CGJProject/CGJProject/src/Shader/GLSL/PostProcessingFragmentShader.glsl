#version 330 core

in vec2 f_texcoord;
uniform sampler2D fbo_texture;
//uniform float offset;
out vec4 color;

void main(void) {
  vec2 texcoord = f_texcoord;
  texcoord.x += sin(texcoord.y * 4.0*2.0*3.14159 /*+ offset*/) / 100.0;
  color = texture2D(fbo_texture, texcoord);
}
