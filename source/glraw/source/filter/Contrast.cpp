#include <glraw/filter/Contrast.h>

#include <QOpenGLShaderProgram>

#include <glraw/Canvas.h>

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
			dst = vec4(clamp(texture(src, v_uv).rgb * factor, vec3(0.f), vec3(1.f)),texture(src, v_uv).a);
		} )";

	const unsigned int DefaultFactor = 0.5;
}

namespace glraw
{

Contrast::Contrast(float factor = DefaultFactor)
	: m_factor(factor)
{
}

Contrast::Contrast(const QVariantMap& cfg)
	: Contrast(FactorFromVariant(cfg, DefaultFactor))
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
