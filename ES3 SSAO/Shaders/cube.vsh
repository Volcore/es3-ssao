#version 300 es
layout(location=0) in vec3 att_position;
uniform mat4 uni_mvp;

void main() {
  gl_Position = uni_mvp * vec4(att_position, 1);
}
