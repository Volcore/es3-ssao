#version 300 es
layout(location=0) in vec3 att_position;
layout(location=1) in vec3 att_normal;
uniform mat4 uni_mvp;
uniform mat3 uni_mv_normal;
uniform mat4 uni_mv;
out highp vec3 var_normal;

void main() {
  gl_Position = uni_mvp * vec4(att_position, 1);
  var_normal = normalize(uni_mv_normal * att_normal);
}
