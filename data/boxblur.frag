#version 150

uniform sampler2D src;
uniform vec2 boxR = vec2(16, 16);

in vec2 v_uv;
out vec4 dst;

void main()
{   
	ivec2 r = ivec2(boxR);
	float w = 1.0 / ((r.x * 2.0 + 1) * (r.y * 2.0 + 1));

	vec2 sizei = vec2(1.0) / vec2(textureSize(src, 0));

    vec4 sum = vec4(0.0);

    for (int x = -r.x; x <= r.x; ++x) 
        for (int y = -r.y; y <= r.y; ++y) 
            sum += texture(src, v_uv + vec2(x, y) * sizei);

    dst = sum * w;
}
