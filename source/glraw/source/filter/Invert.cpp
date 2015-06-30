#include <glraw/filter/Invert.h>

#include <QOpenGLShaderProgram>

namespace
{
	const char * const source =
		R"(#version 150

		uniform sampler2D src;
		uniform bool invert_alpha;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			vec4 color = texture(src, v_uv);
			if( invert_alpha )
			{
				dst = vec4(1) - color;
			}else
			{
				dst = vec4(vec3(1)-color.rgb, color.a);
			}
		} )";

	const bool DefaultInvertAlpha = false;
}

namespace glraw
{

Invert::Invert( bool invert_alpha = DefaultInvertAlpha )
	: m_invertAlpha(invert_alpha)
{
}

Invert::Invert( const QVariantMap& cfg )
	: Invert( AlphaFromVariant(cfg) )
{

}

bool Invert::process( std::unique_ptr<Canvas> & imageData, AssetInformation & info )
{
	return renderShader( imageData, source );
}

void Invert::setUniforms( QOpenGLShaderProgram& program )
{
	program.setUniformValue( "invert_alpha", m_invertAlpha );
}

bool Invert::AlphaFromVariant( const QVariantMap& cfg )
{
	return cfg.value( "invert_alpha", { DefaultInvertAlpha } ).toBool();
}

}