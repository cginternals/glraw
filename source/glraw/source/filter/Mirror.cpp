#include <glraw/filter/Mirror.h>

#include <QOpenGLShaderProgram>

namespace
{
	const char * const source =
		R"(#version 150

		uniform sampler2D src;
		uniform bool horizontal;
		uniform bool vertical;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			dst = texture(src, vec2(horizontal?1.f-v_uv.x:v_uv.x,vertical?1.f-v_uv.y:v_uv.y));
		} )";

	const bool DefaultHorizontal = false;
	const bool DefaultVertical = false;
}

namespace glraw
{

	Mirror::Mirror(bool horizontal = DefaultHorizontal, bool vertical = DefaultVertical)
	: m_horizontal(horizontal)
	, m_vertical(vertical)
{
}

Mirror::Mirror(const QVariantMap& cfg)
	: Mirror(HorizontalFromVariant(cfg), VerticalFromVariant(cfg))
{

}

bool Mirror::process(std::unique_ptr<Canvas> & imageData, AssetInformation & info)
{
	return renderShader( imageData, source );
}

void Mirror::setUniforms(QOpenGLShaderProgram& program)
{
	program.setUniformValue("horizontal", m_horizontal );
	program.setUniformValue("vertical", m_vertical);
}

bool Mirror::HorizontalFromVariant(const QVariantMap& cfg)
{
	return cfg.value("horizontal", { DefaultHorizontal }).toBool();
}
bool Mirror::VerticalFromVariant(const QVariantMap& cfg)
{
	return cfg.value("vertical", { DefaultVertical }).toBool();
}
}