#version 300 es
layout(location=0) in vec3 att_position;
layout(location=1) in vec2 att_texcoord;
uniform mat4 uni_mvp;
out highp vec2 var_texcoord;

void main() {
  gl_Position = uni_mvp * vec4(att_position, 1);
  var_texcoord = att_texcoord;
}
