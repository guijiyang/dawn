SKIP: FAILED

#version 310 es

layout(rg32i) uniform highp writeonly iimage3D arg_0;
void textureStore_d19db4() {
  imageStore(arg_0, ivec3(1), ivec4(1));
}

vec4 vertex_main() {
  textureStore_d19db4();
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
error: Error parsing GLSL shader:
ERROR: 0:3: 'image load-store format' : not supported with this profile: es
ERROR: 0:3: '' : compilation terminated 
ERROR: 2 compilation errors.  No code generated.



#version 310 es
precision highp float;
precision highp int;

layout(rg32i) uniform highp writeonly iimage3D arg_0;
void textureStore_d19db4() {
  imageStore(arg_0, ivec3(1), ivec4(1));
}

void fragment_main() {
  textureStore_d19db4();
}

void main() {
  fragment_main();
  return;
}
error: Error parsing GLSL shader:
ERROR: 0:5: 'image load-store format' : not supported with this profile: es
ERROR: 0:5: '' : compilation terminated 
ERROR: 2 compilation errors.  No code generated.



#version 310 es

layout(rg32i) uniform highp writeonly iimage3D arg_0;
void textureStore_d19db4() {
  imageStore(arg_0, ivec3(1), ivec4(1));
}

void compute_main() {
  textureStore_d19db4();
}

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  compute_main();
  return;
}
error: Error parsing GLSL shader:
ERROR: 0:3: 'image load-store format' : not supported with this profile: es
ERROR: 0:3: '' : compilation terminated 
ERROR: 2 compilation errors.  No code generated.



