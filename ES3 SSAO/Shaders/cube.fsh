#version 300 es
in highp vec3 var_normal;
uniform highp vec3 uni_color;
layout(location=0) out highp vec3 out_color;
layout(location=1) out highp vec3 out_normal;

highp vec3 pack_normal(highp vec3 n) {
  // Pack the normal from [-1..1] to [0..1]
  return n * 0.5 + 0.5;
}

void main() {
  out_color = uni_color;
  out_normal = pack_normal(normalize(var_normal));
}
