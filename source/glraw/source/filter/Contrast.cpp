#include <glraw/filter/Contrast.h>

#include <QOpenGLShaderProgram>

namespace
{
	const char * const source =
		R"(#version 150

		uniform sampler2D src;
		uniform float factor;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			dst = vec4(texture(src, v_uv).rgb * factor, texture(src, v_uv).a);
		} )";

	const float DefaultFactor = 1.0f;
}

namespace glraw
{

Contrast::Contrast(float factor = DefaultFactor)
	: m_factor(factor)
{
}

Contrast::Contrast(const QVariantMap& cfg)
	: Contrast(GetFactor(DefaultFactor, cfg))
{
}

void Contrast::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
{
	program.setUniformValue("factor", m_factor);
}

QString Contrast::fragmentShaderSource(unsigned int pass)
{
	return source;
}
}
