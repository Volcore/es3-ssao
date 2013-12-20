#version 300 es
uniform sampler2D uni_color_tex;
uniform sampler2D uni_normal_tex;
uniform sampler2D uni_depth_tex;

in highp vec2 var_texcoord;

out highp vec3 out_color;

uniform highp vec2 uni_samples[16];
uniform highp mat4 uni_inv_projection;

highp vec3 ReconstructPosition(in highp vec2 coord, in highp float depth) {
  // Projec the screen space position + depth into world space
  highp vec4 vec = vec4(coord.x, coord.y, depth, 1.0);
  vec = vec * 2.0 - 1.0;
  highp vec4 r = uni_inv_projection * vec;
  return r.xyz / r.w;
}

uniform highp int uni_num_samples;
const highp float kRadius = 0.015;
const highp float kDistanceThreshold = 0.5;

void main() {
  highp ivec2 texsize = textureSize(uni_depth_tex, 0);
  highp vec3 color = texture(uni_color_tex, var_texcoord).rgb;
  highp vec3 normal = texture(uni_normal_tex, var_texcoord).rgb;
  normal = normalize(normal * 2.0 - 1.0);
  highp float depth = texture(uni_depth_tex, var_texcoord).r;
  // Reconstruct the position from the depth value
  highp vec3 position = ReconstructPosition(var_texcoord, depth);
  // Sample the AO
  highp float occlusion = 0.0;
  for (int i = 0; i < uni_num_samples; ++i) {
    highp vec2 sample_tex = var_texcoord + (uni_samples[i] * kRadius);
    highp float sample_depth = texture(uni_depth_tex, sample_tex).r;
    highp vec3 sample_pos = ReconstructPosition(sample_tex, sample_depth);
    highp vec3 diff_vec = sample_pos - position;
    highp float distance = length(diff_vec);
    highp vec3 sample_dir = diff_vec * 1.0/distance;
    highp float cosine = max(dot(normal, sample_dir), 0.0);
    // a = distance function
    highp float a = 1.0 - smoothstep(kDistanceThreshold, kDistanceThreshold * 2.0, distance);
    // b = dot-Product
    highp float b = cosine;
    occlusion += (b * a);
//    occlusion += b;
  }
  // Normalize occlusion
  occlusion = 1.0 - occlusion / float(uni_num_samples);
  out_color = color * occlusion;
  // Debug visualize the depth buffer
//  out_color = vec3(depth, linearizeDepth(depth, uni_inv_projection), depth);
  // Debug visualize the normals
//  out_color = normal;
  // Debug visualize the position
//  out_color = position;
  // Debug visualize the color buffer
//  out_color = color;
  // Debug visualize the texcoords
//  out_color = vec3(var_texcoord.x, var_texcoord.y, 0.0);
}
