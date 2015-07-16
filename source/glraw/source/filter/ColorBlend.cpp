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
			if( blend_mode == 0 ) // Mix
			{
				dst = (texel+color) * 0.5;
			}
			else if( blend_mode == 1 ) // Additive
			{
				dst = (texel+color);
			}
			else if( blend_mode == 2 ) // Subtractive
			{
				dst = (texel-color);
			}	
			else if( blend_mode == 3 ) // Difference
			{
				dst = (color-texel);
			}	
			else if( blend_mode == 4 ) // Multiply
			{
				dst = (texel*color);
			}
			else if( blend_mode == 5 ) // Divide
			{
				dst = (texel/color);
			}
			else if( blend_mode == 6 ) // Divisor
			{
				dst = (color/texel);
			}
			else if( blend_mode == 7 ) // Minimum
			{
				dst = min(texel,color);
			}	
			else if( blend_mode == 8 ) // Maximum
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

bool ColorBlend::process( std::unique_ptr<Canvas> & imageData, AssetInformation & info )
{
	return renderShader( imageData, source );
}

void ColorBlend::setUniforms( QOpenGLShaderProgram& program )
{
	program.setUniformValue("mode", static_cast<int>(m_blendMode));
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