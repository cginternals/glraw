#include <glraw/filter/Brightness.h>

#include <QOpenGLShaderProgram>

#include <glraw/Canvas.h>

namespace
{
	const char * const source =
		R"(#version 150

		uniform sampler2D src;
		uniform float amount;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			dst = vec4(clamp(texture(src, v_uv).rgb + vec3(amount), vec3(0.f), vec3(1.f)),texture(src, v_uv).a);
		} )";

	const float DefaultAmount = 0.5;
}

namespace glraw
{

Brightness::Brightness(float amount = DefaultAmount)
	: m_amount(amount)
{
}

Brightness::Brightness(const QVariantMap& cfg)
	: Brightness(AmountFromVariant(cfg, DefaultAmount))
{
}

void Brightness::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
{
	program.setUniformValue("amount", m_amount);
}

QString Brightness::fragmentShaderSource(unsigned int pass)
{
	return source;
}

float Brightness::AmountFromVariant(const QVariantMap& cfg, float default_value)
{
	return cfg.value("amount", { default_value }).toFloat();
}

}
