#version 150

uniform sampler2D src;

in vec2 v_uv;
layout(location = 0) out vec4 dst;

void main()
{   
	vec4 color = texture(src, v_uv);
	dst = vec4(vec3(0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b), color.a);
}
