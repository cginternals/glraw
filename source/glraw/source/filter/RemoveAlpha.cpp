#include <glraw/filter/RemoveAlpha.h>

#include <QOpenGLShaderProgram>

namespace
{
	const char * const source =
		R"(#version 150

		uniform sampler2D src;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			dst = vec4(texture(src, v_uv).rgb, 1);
		} )";
}

namespace glraw
{

RemoveAlpha::RemoveAlpha(const QVariantMap& in)
{
}

QString RemoveAlpha::fragmentShaderSource(unsigned int pass)
{
	return source;
}

}