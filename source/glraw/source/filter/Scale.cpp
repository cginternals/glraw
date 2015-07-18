#include <glraw/filter/Scale.h>

#include <QOpenGLShaderProgram>

#include <glraw/Canvas.h>
#include <glraw/AssetInformation.h>

namespace
{
	const char * const source =
		R"(#version 150

		uniform sampler2D src;
		uniform int width;
		uniform int height;
		uniform bool bilinear;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			if(bilinear)
			{
				ivec2 size = textureSize(src, 0);
				float dX = v_uv.x/size.x;
				float dY = v_uv.y/size.y;

				vec2 texCoords = vec2(dX*width, dY*height);
				vec2 ratio = fract(texCoords);

				vec4 c1 = texture(src, v_uv);
				vec4 c2 = texture(src, v_uv + vec2(dX,0.f));
				vec4 c3 = texture(src, v_uv + vec2(0.f,dY));
				vec4 c4 = texture(src, v_uv + vec2(dX,dY));

				dst = mix(mix(c1,c2,ratio.x), mix(c3,c4,ratio.x), ratio.y);
			}
			else
			{
				dst = texture(src, v_uv);
			}
		} )";

	const float DefaultScale = 1.0f;
	const int DefaultWidth = 500;
	const int DefaultHeight = 500;
	bool DefaultBilinear = false;
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
	, m_width(WidthFromVariant(cfg))
	, m_height(HeightFromVariant(cfg))
	, m_scale(ScaleFromVariant(cfg))
	, m_bilinear(BilinearFromVariant(cfg))
{
}

void Scale::updateAssetInformation(AssetInformation & info)
{
	info.setProperty("width", out_width);
	info.setProperty("height", out_height);
}

void Scale::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
{
	program.setUniformValue("nearest", m_bilinear);
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

Scale::ScaleMode Scale::ModeFromVariant(const QVariantMap& cfg)
{
	return static_cast<ScaleMode>(cfg.value("mode", { static_cast<int>(ScaleMode::Default) }).toInt());
}

float Scale::ScaleFromVariant(const QVariantMap& cfg)
{
	return cfg.value("scale", { DefaultScale }).toFloat();
}
unsigned int Scale::WidthFromVariant(const QVariantMap& cfg)
{
	return cfg.value("width", { DefaultWidth }).toInt();
}
unsigned int Scale::HeightFromVariant(const QVariantMap& cfg)
{
	return cfg.value("height", { DefaultHeight }).toInt();
}
bool Scale::BilinearFromVariant(const QVariantMap& cfg)
{
	return cfg.value("bilinear", { DefaultBilinear }).toBool();
}

}
