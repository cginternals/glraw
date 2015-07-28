#include <glraw/filter/ColorBlend.h>

#include <QOpenGLShaderProgram>

namespace
{
	const char * const source =
		R"(#version 150

		uniform sampler2D src;
		uniform float factor;
		uniform int mode;
		uniform vec4 color;

		in vec2 v_uv;
		out vec4 dst;

		const int Mix			= 0;
		const int Additive		= 1;
		const int Subtractive	= 2;
		const int Difference	= 3;
		const int Multiply		= 4;
		const int Divide		= 5;
		const int Minimum		= 6;
		const int Maximum		= 7;

		void main()
		{   
			vec4 texel = texture(src, v_uv);
			if( mode == Mix )
			{
				dst = (texel+color) * 0.5;
			}
			else if( mode == Additive )
			{
				dst = (texel+color);
			}
			else if( mode == Subtractive )
			{
				dst = (texel-color);
			}	
			else if( mode == Difference )
			{
				dst = abs(color-texel);
			}	
			else if( mode == Multiply )
			{
				dst = (texel*color);
			}
			else if( mode == Divide )
			{
				dst = (texel/color);
			}
			else if( mode == Minimum )
			{
				dst = min(texel,color);
			}	
			else if( mode == Maximum )
			{
				dst = max(texel,color);
			}

			dst = mix(texel,dst,factor);
		} )";

	const QVector4D DefaultColor = QVector4D(1.0f ,1.0f, 1.0f, 1.0f);
	const float DefaultFactor = 1.0f;
}

namespace glraw
{

ColorBlend::ColorBlend(BlendMode mode = BlendMode::Default, const QVector4D& color = DefaultColor, float factor = DefaultFactor)
	: m_blendMode(mode)
	, m_color(color)
	, m_factor(factor)
{
}

ColorBlend::ColorBlend( const QVariantMap& cfg )
	: ColorBlend(GetMode(cfg), GetColor(DefaultColor, cfg), GetFactor(DefaultFactor, cfg))
{
}

void ColorBlend::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
{
	program.setUniformValue("mode", static_cast<int>(m_blendMode));
	program.setUniformValue("factor", m_factor);
	program.setUniformValue("color", m_color);
}

QString ColorBlend::fragmentShaderSource(unsigned int pass)
{
	return source;
}

BlendMode ColorBlend::GetMode(const QVariantMap& cfg)
{
	auto value = cfg.value("mode", { static_cast<int>(BlendMode::Default) }).toInt();
	return static_cast<BlendMode>(value);
}

}