#include <glraw/filter/Grayscale.h>

#include <QOpenGLShaderProgram>

#include <glraw/Canvas.h>

namespace
{
const char * const source =
		R"(#version 150

		uniform sampler2D src;
		uniform vec3 factor;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			vec4 color = texture(src, v_uv);
			dst = vec4(vec3(factor.r * color.r + factor.g * color.g + factor.b * color.b), color.a);
		} )";
}

namespace glraw
{

Grayscale::Grayscale(GrayscaleFactor in)
	: Grayscale(FactorFromMode(in))
{
}

Grayscale::Grayscale(const QVector3D& factor)
	: m_conversionFactor(factor)
{
}

Grayscale::Grayscale(const QVariantMap& cfg)
	: Grayscale(FactorFromVariant(cfg))
{
}

bool Grayscale::process(std::unique_ptr<Canvas> & imageData, AssetInformation & info)
{
	return renderShader(imageData, source);
}

void Grayscale::setUniforms(QOpenGLShaderProgram& program)
{
	program.setUniformValue("factor", m_conversionFactor);
}

QVector3D Grayscale::FactorFromMode(GrayscaleFactor in)
{
	switch (in)
	{
	default:
		qCritical("Invalid GrayscaleFactor enum used!");

	case GrayscaleFactor::Classic:
		return { 0.299f, 0.587f, 0.114f };
	case GrayscaleFactor::Modern:
		return { 0.2126f, 0.7152f, 0.0722f };
	}
}

QVector3D Grayscale::FactorFromVariant(const QVariantMap& cfg)
{
	auto it = cfg.find("mode");
	if (it != cfg.end())
	{
		auto mode = static_cast<GrayscaleFactor>(it->toInt());
		return FactorFromMode(mode);
	}
	else
	{
		const auto def = FactorFromMode(GrayscaleFactor::Default);
		float r = cfg.value("r", {def.x()}).toFloat();
		float g = cfg.value("g", {def.y()}).toFloat();
		float b = cfg.value("b", {def.z()}).toFloat();

		return QVector3D(r, g, b);
	}
}

}
