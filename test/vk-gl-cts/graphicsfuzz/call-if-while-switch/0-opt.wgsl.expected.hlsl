SKIP: FAILED

cbuffer cbuffer_x_6 : register(b0, space0) {
  uint4 x_6[1];
};
static float4 x_GLF_color = float4(0.0f, 0.0f, 0.0f, 0.0f);

void main_1() {
  int data[10] = (int[10])0;
  int x_40 = 0;
  int x_40_phi = 0;
  int x_11_phi = 0;
  const int x_7 = data[1];
  const int x_10 = ((1 < x_7) ? 2 : 1);
  x_40_phi = 1;
  x_11_phi = x_10;
  while (true) {
    int x_54 = 0;
    int x_41 = 0;
    int x_41_phi = 0;
    x_40 = x_40_phi;
    const int x_11 = x_11_phi;
    if ((x_11 < 3)) {
    } else {
      break;
    }
    int x_54_phi = 0;
    const int x_8 = (x_11 + 1);
    const float x_47 = asfloat(x_6[0].x);
    x_54_phi = x_40;
    switch(int(x_47)) {
      case 78: {
        x_GLF_color = float4(1.0f, 1.0f, 1.0f, 1.0f);
        /* fallthrough */
      }
      case 19: {
        x_54_phi = asint((x_40 + asint(1)));
        /* fallthrough */
      }
      case 23:
      case 38: {
        x_54 = x_54_phi;
        x_41_phi = x_54;
        {
          x_41 = x_41_phi;
          x_40_phi = x_41;
          x_11_phi = x_8;
        }
        continue;
        break;
      }
      default: {
        x_41_phi = x_40;
        {
          x_41 = x_41_phi;
          x_40_phi = x_41;
          x_11_phi = x_8;
        }
        continue;
        break;
      }
    }
    x_41_phi = 0;
    {
      x_41 = x_41_phi;
      x_40_phi = x_41;
      x_11_phi = x_8;
    }
  }
  data[x_40] = 1;
  x_GLF_color = float4(1.0f, 0.0f, 0.0f, 1.0f);
  return;
}

struct main_out {
  float4 x_GLF_color_1;
};
struct tint_symbol {
  float4 x_GLF_color_1 : SV_Target0;
};

tint_symbol main() {
  main_1();
  const main_out tint_symbol_1 = {x_GLF_color};
  const tint_symbol tint_symbol_3 = {tint_symbol_1.x_GLF_color_1};
  return tint_symbol_3;
}
C:\src\tint\test\Shader@0x000001DB60E578A0(47,9-17): error X3708: continue cannot be used in a switch

