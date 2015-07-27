#include <glraw/filter/Brightness.h>

#include <QOpenGLShaderProgram>

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
			dst = vec4(texture(src, v_uv).rgb + vec3(amount), texture(src, v_uv).a);
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
	: Brightness(GetFactor(DefaultAmount,cfg))
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

}
