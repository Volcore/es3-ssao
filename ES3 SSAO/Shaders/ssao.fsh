#version 300 es
uniform sampler2D uni_color_tex;
uniform sampler2D uni_normal_tex;
uniform sampler2D uni_depth_tex;

in highp vec2 var_texcoord;

out highp vec3 out_color;

uniform highp vec3 uni_samples[16];
uniform highp mat4 uni_projection;
uniform highp mat4 uni_inv_projection;

const highp vec2 poisson16[] = vec2[](    // These are the Poisson Disk Samples
                                vec2( -0.94201624,  -0.39906216 ),
                                vec2(  0.94558609,  -0.76890725 ),
                                vec2( -0.094184101, -0.92938870 ),
                                vec2(  0.34495938,   0.29387760 ),
                                vec2( -0.91588581,   0.45771432 ),
                                vec2( -0.81544232,  -0.87912464 ),
                                vec2( -0.38277543,   0.27676845 ),
                                vec2(  0.97484398,   0.75648379 ),
                                vec2(  0.44323325,  -0.97511554 ),
                                vec2(  0.53742981,  -0.47373420 ),
                                vec2( -0.26496911,  -0.41893023 ),
                                vec2(  0.79197514,   0.19090188 ),
                                vec2( -0.24188840,   0.99706507 ),
                                vec2( -0.81409955,   0.91437590 ),
                                vec2(  0.19984126,   0.78641367 ),
                                vec2(  0.14383161,  -0.14100790 )
                               );

highp float linearizeDepth(in highp float depth, in highp mat4 projMatrix) {
	return projMatrix[3][2] / (depth - projMatrix[2][2]);
}

highp vec3 ReconstructPosition(in highp vec2 coord, in highp float depth) {
  // Projec the screen space position + depth into world space
  highp vec4 vec = vec4(coord.x, coord.y, depth, 1.0);
  vec = vec * 2.0 - 1.0;
  highp vec4 r = uni_inv_projection * vec;
  return r.xyz / r.w;
}

void main() {
  highp ivec2 texsize = textureSize(uni_depth_tex, 0);
  highp vec3 color = texture(uni_color_tex, var_texcoord).rgb;
  highp vec3 normal = texture(uni_normal_tex, var_texcoord).rgb;
  normal = normalize(normal * 2.0 - 1.0);
  highp float depth = texture(uni_depth_tex, var_texcoord).r;
  // Reconstruct the position from the depth value
  highp vec3 position = ReconstructPosition(var_texcoord, depth);

  // Output the result
  out_color = color;

//  // Sample
//  highp float occlusion = 0.0;
//  for (int i = 0; i < 5; ++i) {
//    highp vec3 sample_vec = uni_samples[i];
//    sample_vec = dot(sample_vec, normal) < 0.0 ? -sample_vec : sample_vec;
//    highp vec3 s = position + uni_samples[i] * 0.1;
//    highp vec4 offset = vec4(s, 1.0);
//    offset = uni_projection * offset;
//    offset.xy /= offset.w;
//    offset.xy = offset.xy * 0.5 + 0.5;
//    highp float sampleDepth = texture(uni_position_tex, offset.xy).z;
////    highp float rangeCheck= abs(position.z - sampleDepth) < uRadius ? 1.0 : 0.0;
////    occlusion += (sampleDepth <= s.z ? 1.0 : 0.0) * rangeCheck;
//    occlusion += sampleDepth <= s.z ? 1.0 : 0.0;
//  }
//  // Normalize occlusion
//  occlusion = 1.0 - occlusion / 5.0;
//  out_color = color * occlusion;

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
