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

		void main()
		{   
			vec4 texel = texture(src, v_uv);
			if( mode == 0 ) // Mix
			{
				dst = (texel+color) * 0.5;
			}
			else if( mode == 1 ) // Additive
			{
				dst = (texel+color);
			}
			else if( mode == 2 ) // Subtractive
			{
				dst = (texel-color);
			}	
			else if( mode == 3 ) // Difference
			{
				dst = abs(color-texel);
			}	
			else if( mode == 4 ) // Multiply
			{
				dst = (texel*color);
			}
			else if( mode == 5 ) // Divide
			{
				dst = (texel/color);
			}
			else if( mode == 6 ) // Divisor
			{
				dst = (color/texel);
			}
			else if( mode == 7 ) // Minimum
			{
				dst = min(texel,color);
			}	
			else if( mode == 8 ) // Maximum
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
	: ColorBlend(ModeFromVariant(cfg), ColorFromVariant(cfg), FactorFromVariant(cfg, DefaultFactor))
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

BlendMode ColorBlend::ModeFromVariant(const QVariantMap& cfg)
{
	int value = cfg.value("mode", { static_cast<int>(BlendMode::Default) }).toInt();
	return static_cast<BlendMode>(value);
}

QVector4D ColorBlend::ColorFromVariant(const QVariantMap& cfg)
{
	float r = cfg.value("r", { DefaultColor.x() }).toFloat();
	float g = cfg.value("g", { DefaultColor.y() }).toFloat();
	float b = cfg.value("b", { DefaultColor.z() }).toFloat();
	float a = cfg.value("a", { DefaultColor.w() }).toFloat();

	return {r, g, b, a};
}

}