fn bitcast_b77573() {
  var res : vec2<f32> = bitcast<vec2<f32>>(vec2<i32>(1i));
  prevent_dce = res;
}

@group(2) @binding(0) var<storage, read_write> prevent_dce : vec2<f32>;

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  bitcast_b77573();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  bitcast_b77573();
}

@compute @workgroup_size(1)
fn compute_main() {
  bitcast_b77573();
}
