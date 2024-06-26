#version 310 es

layout(binding = 0, std430) buffer prevent_dce_block_ssbo {
  float inner;
} prevent_dce;

void sin_b78c91() {
  float arg_0 = 1.57079637050628662109f;
  float res = sin(arg_0);
  prevent_dce.inner = res;
}

vec4 vertex_main() {
  sin_b78c91();
  return vec4(0.0f);
}

void main() {
  gl_PointSize = 1.0;
  vec4 inner_result = vertex_main();
  gl_Position = inner_result;
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  return;
}
#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430) buffer prevent_dce_block_ssbo {
  float inner;
} prevent_dce;

void sin_b78c91() {
  float arg_0 = 1.57079637050628662109f;
  float res = sin(arg_0);
  prevent_dce.inner = res;
}

void fragment_main() {
  sin_b78c91();
}

void main() {
  fragment_main();
  return;
}
#version 310 es

layout(binding = 0, std430) buffer prevent_dce_block_ssbo {
  float inner;
} prevent_dce;

void sin_b78c91() {
  float arg_0 = 1.57079637050628662109f;
  float res = sin(arg_0);
  prevent_dce.inner = res;
}

void compute_main() {
  sin_b78c91();
}

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  compute_main();
  return;
}
