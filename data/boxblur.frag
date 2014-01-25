#version 330

uniform sampler2D src;

in vec2 v_uv;
layout(location = 0) out vec4 dst;


const int blurR = 16;
const float w = 1.0 / pow(blurR * 2.0 + 1.0, 2.0);

void main()
{   
	vec2 sizei = vec2(1.0) / vec2(textureSize(src, 0));

    vec4 sum = vec4(0.0);

    for (int x = -blurR; x <= blurR; ++x) 
        for (int y = -blurR; y <= blurR; ++y) 
            sum += texture(src, v_uv + vec2(x, y) * sizei);

    dst = sum * w;
}
