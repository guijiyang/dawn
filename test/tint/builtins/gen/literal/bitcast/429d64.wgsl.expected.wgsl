enable f16;

fn bitcast_429d64() {
  var res : vec4<f16> = bitcast<vec4<f16>>(vec2<f32>(1.0f));
  prevent_dce = res;
}

@group(2) @binding(0) var<storage, read_write> prevent_dce : vec4<f16>;

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  bitcast_429d64();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  bitcast_429d64();
}

@compute @workgroup_size(1)
fn compute_main() {
  bitcast_429d64();
}
