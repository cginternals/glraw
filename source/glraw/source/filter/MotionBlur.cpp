#include <glraw/filter/MotionBlur.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <QOpenGLShaderProgram>

namespace
{
	const char * const source =
		R"(#version 150

		uniform sampler2D src;
		uniform int size;
		uniform float factor;
		uniform vec2 velocity;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			vec2 stepSize = velocity/textureSize(src, 0);
			
			vec2 coord = v_uv;
			for(int i=0; i < size; ++i, coord += stepSize)
			{
				dst += texture(src,coord);
			}
			dst = mix(texture(src, v_uv), dst / size, factor);
		} )";

	const unsigned int DefaultSize = 10;
	const float DefaultAngle = 0.0f;
	const float DefaultFactor = 1.0f;
}

namespace glraw
{

MotionBlur::MotionBlur(unsigned int size = DefaultSize, float angle = DefaultAngle, float factor = DefaultFactor)
	: m_size(VerifySize(size))
	, m_angle(angle)
	, m_factor(factor)
{
}

MotionBlur::MotionBlur(const QVariantMap& cfg)
	: MotionBlur(GetSize(DefaultSize, cfg), GetAngle(cfg), GetFactor(DefaultFactor, cfg))
{
}

void MotionBlur::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
{
	program.setUniformValue("size", m_size);
	program.setUniformValue("factor", m_factor);
	program.setUniformValue("velocity", QVector2D(cos(m_angle),sin(m_angle)));
}

QString MotionBlur::fragmentShaderSource(unsigned int pass)
{
	return source;
}

float MotionBlur::GetAngle(const QVariantMap & cfg)
{
	if (cfg.contains("angled"))
	{
		return cfg.value("angled").toFloat() * M_PI / 180.f;
	}
	else
	{
		return cfg.value("angle").toFloat();
	}
}

}
