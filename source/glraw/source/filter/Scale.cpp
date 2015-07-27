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

	const float DefaultScale = 1.0f;
	const unsigned int DefaultWidth = 500;
	const unsigned int DefaultHeight = 500;
	const bool DefaultBilinear = false;
}

namespace glraw
{

Scale::Scale(ScaleMode mode = ScaleMode::Default, unsigned int width = DefaultWidth, unsigned int height = DefaultHeight, float scale = DefaultScale, bool bilinear = DefaultBilinear)
	: m_mode(mode)
	, m_width(width)
	, m_height(height)
	, m_scale(scale)
	, m_bilinear(bilinear)
{
}

Scale::Scale(const QVariantMap& cfg)
	: m_mode(ModeFromVariant(cfg))
	, m_width(Get("width", DefaultWidth, cfg))
	, m_height(Get("height", DefaultHeight, cfg))
	, m_scale(Get("scale", DefaultScale, cfg))
	, m_bilinear(Get("bilinear", DefaultBilinear, cfg))
{
}

void Scale::updateAssetInformation(AssetInformation & info)
{
	info.setProperty("width", out_width);
	info.setProperty("height", out_height);
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

	switch(m_mode)
	{
	case ScaleMode::Absolute:
		width = m_width;
		height = m_height;
		break;
	case ScaleMode::RatioX:
		height = height * static_cast<float>(m_width) / width;
		width = m_width;
		break;
	case ScaleMode::RatioY:
		width = width * static_cast<float>(m_height) / height;
		height = m_height;
		break;
	default:
		width = (int)(width*m_scale);
		height = (int)(height*m_scale);
		break;
	}

	GLuint buffer_texture;
	m_gl->glGenTextures(1, &buffer_texture);
	m_gl->glBindTexture(GL_TEXTURE_2D, buffer_texture);
	m_gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	m_gl->glViewport(0, 0, width, height);

	out_width = width;
	out_height = height;

	return buffer_texture;
}

void Scale::bindTexture(unsigned int unit, unsigned int tex)
{
	m_gl->glActiveTexture(GL_TEXTURE0 + unit);
	m_gl->glBindTexture(GL_TEXTURE_2D, tex);

	if(m_bilinear)
	{
		m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}else
	{
		m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Scale::ScaleMode Scale::ModeFromVariant(const QVariantMap& cfg)
{
	return static_cast<ScaleMode>(cfg.value("mode", { static_cast<int>(ScaleMode::Default) }).toInt());
}

}
