[[group(1), binding(1)]] var arg_1 : texture_cube<u32>;

[[group(1), binding(2)]] var arg_2 : sampler;

fn textureGather_e1b67d() {
  var res : vec4<u32> = textureGather(1, arg_1, arg_2, vec3<f32>());
}

[[stage(vertex)]]
fn vertex_main() -> [[builtin(position)]] vec4<f32> {
  textureGather_e1b67d();
  return vec4<f32>();
}

[[stage(fragment)]]
fn fragment_main() {
  textureGather_e1b67d();
}

[[stage(compute), workgroup_size(1)]]
fn compute_main() {
  textureGather_e1b67d();
}
