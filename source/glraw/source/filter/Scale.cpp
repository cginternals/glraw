#include <glraw/filter/Scale.h>

#include <QOpenGLShaderProgram>

#include <glraw/Canvas.h>
#include <glraw/AssetInformation.h>

namespace
{
	const char * const source =
		R"(#version 150

		uniform sampler2D src;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			dst = texture(src, v_uv);
		} )";

	const float DefaultScale = 2.0f;
	const unsigned int DefaultSize = 500;
	const bool DefaultBilinear = true;
	const bool DefaulXAxis = true;
}

namespace glraw
{

Scale::Scale(float scale = DefaultScale, bool bilinear = DefaultBilinear)
	: m_mode(ScaleMode::Relative)
	, m_size(scale, scale)
	, m_bilinear(bilinear)
{
}

Scale::Scale(unsigned int width, unsigned int height, bool bilinear = DefaultBilinear)
	: m_mode(ScaleMode::Absolute)
	, m_size(width, height)
	, m_bilinear(bilinear)
{
}

Scale::Scale(unsigned int side, bool x_axis = DefaulXAxis, bool bilinear = DefaultBilinear)
	: m_mode(x_axis ? ScaleMode::RatioX : ScaleMode::RatioY)
	, m_size(side, side)
	, m_bilinear(bilinear)
{
}

Scale::Scale(const QVariantMap& cfg)
	: m_mode(ModeFromVariant(cfg))
	, m_size(GetSize(m_mode, cfg))
	, m_bilinear(Get("bilinear", DefaultBilinear, cfg))
{
}

void Scale::updateAssetInformation(AssetInformation & info)
{
	info.setProperty("width", m_output.first);
	info.setProperty("height", m_output.second);
}

QString Scale::fragmentShaderSource(unsigned int pass)
{
	return source;
}

int Scale::createWorkingTexture(unsigned int prototype)
{
	GLint width, height;
	m_gl->glBindTexture(GL_TEXTURE_2D, prototype);
	m_gl->glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	m_gl->glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

	auto cache = calculateSize(width, height);
	m_output = { static_cast<int>(cache.x()), static_cast<int>(cache.y()) };

	GLuint buffer_texture;
	m_gl->glGenTextures(1, &buffer_texture);
	m_gl->glBindTexture(GL_TEXTURE_2D, buffer_texture);
	m_gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_output.first, m_output.second, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	m_gl->glViewport(0, 0, m_output.first, m_output.second);

	return buffer_texture;
}

QVector2D Scale::calculateSize(int width, int height) const
{
	switch(m_mode)
	{
	case ScaleMode::Absolute:
		return { m_size.x(), m_size.y() };

	default:
		qDebug("Invalid ScaleMode enum used.");

	case ScaleMode::Relative:
		return { width * m_size.x(), height * m_size.y() };

	case ScaleMode::RatioX:
		return { m_size.x(), height * m_size.x() / width };

	case ScaleMode::RatioY:
		return { width * m_size.y() / height, m_size.y() };
	}
}

void Scale::bindTexture(unsigned int unit, unsigned int tex)
{
	m_gl->glActiveTexture(GL_TEXTURE0 + unit);
	m_gl->glBindTexture(GL_TEXTURE_2D, tex);

	m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_bilinear ? GL_LINEAR : GL_NEAREST);
	m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_bilinear ? GL_LINEAR : GL_NEAREST);

	m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

ScaleMode Scale::ModeFromVariant(const QVariantMap& cfg)
{
	return static_cast<ScaleMode>(cfg.value("mode", { static_cast<int>(ScaleMode::Default) }).toInt());
}

QVector2D Scale::GetSize(ScaleMode mode, const QVariantMap& cfg)
{
	switch(mode)
	{
	case ScaleMode::Absolute:
		return 
		{ 
			static_cast<float>(Get("width", DefaultSize, cfg)),
			static_cast<float>(Get("height", DefaultSize, cfg))
		};

	default:
		qDebug("Invalid ScaleMode enum used.");

	case ScaleMode::Relative:
		{
			float scale = Get("scale", DefaultScale, cfg);
			return { Get("scale_x", scale, cfg), Get("scale_y", scale, cfg) };
		}

	case ScaleMode::RatioX:
		return { static_cast<float>(Get("width", DefaultSize, cfg)), 0.0f };

	case ScaleMode::RatioY:
		return { 0.0f, static_cast<float>(Get("height", DefaultSize, cfg)) };
	}
}

}
