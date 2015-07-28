#include <glraw/filter/Invert.h>

namespace
{
	const char * const source =
		R"(#version 150

		uniform sampler2D src;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			vec4 color = texture(src, v_uv);
			dst = vec4(vec3(1)-color.rgb, color.a);
		} )";

	const bool DefaultInvertAlpha = false;
}

namespace glraw
{

Invert::Invert( const QVariantMap& cfg )
{
}

QString Invert::fragmentShaderSource(unsigned int pass)
{
	return source;
}

}