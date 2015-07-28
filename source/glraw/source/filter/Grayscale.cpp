#include <glraw/filter/Grayscale.h>

#include <QOpenGLShaderProgram>

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

Grayscale::Grayscale(GrayscaleFactor in = GrayscaleFactor::Default)
	: Grayscale(GetFactor(in))
{
}

Grayscale::Grayscale(const QVector3D& factor)
	: m_conversionFactor(factor)
{
}

Grayscale::Grayscale(const QVariantMap& cfg)
	: Grayscale(GetFactor(cfg))
{
}

void Grayscale::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
{
	program.setUniformValue("factor", m_conversionFactor);
}

QString Grayscale::fragmentShaderSource(unsigned int pass)
{
	return source;
}

QVector3D Grayscale::GetFactor(GrayscaleFactor in)
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

QVector3D Grayscale::GetFactor(const QVariantMap& cfg)
{
	auto it = cfg.find("mode");
	if (it != cfg.end())
	{
		auto mode = static_cast<GrayscaleFactor>(it->toInt());
		return GetFactor(mode);
	}
	else
	{
		const auto default_hue = GetFactor(GrayscaleFactor::Default);
		return GetColor(default_hue, cfg);
	}
}

}
