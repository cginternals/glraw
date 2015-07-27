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
		uniform vec2 velocity;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			vec2 stepSize = velocity/textureSize(src, 0);
			
			dst = texture(src, v_uv);
			vec2 coord = v_uv+stepSize;
			for(int i=1; i < size; ++i,coord+=stepSize)
			{
				dst += texture(src,coord);
			}
			dst /= size;
		} )";

	const unsigned int DefaultSize = 10;
	const float DefaultAngle = 0.f;
}

namespace glraw
{

MotionBlur::MotionBlur(unsigned int size = DefaultSize, float angle = DefaultAngle)
	: m_size(VerifySize(size))
	, m_angle(angle)
{
}

MotionBlur::MotionBlur(const QVariantMap& cfg)
	: MotionBlur(GetSize(DefaultSize, cfg), GetAngle(DefaultAngle, cfg))
{
}

void MotionBlur::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
{
	program.setUniformValue("size", m_size);
	program.setUniformValue("velocity", QVector2D(cos(m_angle),sin(m_angle)));
}

QString MotionBlur::fragmentShaderSource(unsigned int pass)
{
	return source;
}

float MotionBlur::GetAngle(float default_value, const QVariantMap & cfg)
{
	if (cfg.contains("angled"))
	{
		return cfg.value("angled", { default_value }).toFloat() * M_PI / 180.f;
	}
	else
	{
		return cfg.value("angle", { default_value }).toFloat();
	}
}

}
