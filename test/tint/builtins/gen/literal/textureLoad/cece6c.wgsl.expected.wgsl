enable chromium_internal_graphite;

@group(1) @binding(0) var arg_0 : texture_storage_3d<r8unorm, read>;

fn textureLoad_cece6c() {
  var res : vec4<f32> = textureLoad(arg_0, vec3<i32>(1i));
  prevent_dce = res;
}

@group(2) @binding(0) var<storage, read_write> prevent_dce : vec4<f32>;

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  textureLoad_cece6c();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  textureLoad_cece6c();
}

@compute @workgroup_size(1)
fn compute_main() {
  textureLoad_cece6c();
}
