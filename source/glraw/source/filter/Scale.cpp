#include <glraw/filter/Scale.h>

#include <QOpenGLShaderProgram>

#include <glraw/Canvas.h>
#include <glraw/AssetInformation.h>

namespace
{
	const char * const sourceBilinear =
		R"(#version 150

		uniform sampler2D src;
		uniform int width;
		uniform int height;
		uniform bool mode;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			ivec2 size = textureSize(src, 0);
			float dX = v_uv.x/size.x;
			float dY = v_uv.y/size.y;

			vec2 texCoords = vec2(dX*width, dY*height);
			vec2 ratio = fract(texCoords);

			if(mode)
			{
				dst = texture(src, texCoords);
				return;
			}

			vec4 c1 = texture(src, texCoords);
			vec4 c2 = texture(src, texCoords + vec2(dX,0.f));
			vec4 c3 = texture(src, texCoords + vec2(0.f,dY));
			vec4 c4 = texture(src, texCoords + vec2(dX,dY));

			dst = mix(mix(c1,c2,ratio.x), mix(c3,c4,ratio.x), ratio.y);

			dst = vec4(1.0f);

		} )";

	const char * const sourceFast =
		R"(#version 150

		uniform sampler2D src;
		uniform int width;
		uniform int height;
		uniform bool mode;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			ivec2 size = textureSize(src, 0);
			float dX = v_uv.x/size.x;
			float dY = v_uv.y/size.y;

			vec2 texCoords = vec2(dX*width, dY*height);
			vec2 ratio = fract(texCoords);

			dst = texture(src, texCoords);

		} )";

	const float DefaultScale = 1.0f;
	const int DefaultWidth = 500;
	const int DefaultHeight = 500;
	bool DefaultBilinear = true;
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

bool Scale::process(std::unique_ptr<Canvas> & imageData, AssetInformation & info)
{
	imageData->makeContext();
	QOpenGLShaderProgram program;
	if (m_bilinear)
	{
		if (!createProgram(program, sourceBilinear))
		{
			qCritical("Shader Error!");
			return 0;
		}
	}
	else
	{
		if (!createProgram(program, sourceFast))
		{
			qCritical("Shader Error!");
			return 0;
		}
	}
	GLuint texture = imageData->texture();
	auto m_gl = imageData->gl();


	m_gl->glBindTexture(GL_TEXTURE_2D, texture);

	GLuint processedTexture;
	GLint w, h;
	int width, height;

	m_gl->glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
	m_gl->glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
	switch (m_mode)
	{
	case 0:
		width = m_width;
		height = m_height;
		break;
	case 2:
		width = m_width;
		height = m_width / width;
		break;
	case 3:
		width = m_height / height;
		height = height;
		break;
	default:
		width = (int)(m_width*m_scale);
		height = (int)(m_height*m_scale);
		break;
	}

	//info.setProperty("width", width);
	//info.setProperty("height", height);


	m_gl->glGenTextures(1, &processedTexture);
	m_gl->glBindTexture(GL_TEXTURE_2D, processedTexture);
	m_gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

	GLuint fbo;
	m_gl->glGenFramebuffers(1, &fbo);
	m_gl->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	m_gl->glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, processedTexture, 0);

	GLuint vao;
	m_gl->glGenVertexArrays(1, &vao);
	m_gl->glBindVertexArray(vao);

	static const float rawv[] = { +1.f, -1.f, +1.f, +1.f, -1.f, -1.f, -1.f, +1.f };

	GLuint buffer;
	m_gl->glGenBuffers(1, &buffer);
	m_gl->glBindBuffer(GL_ARRAY_BUFFER, buffer);
	m_gl->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, rawv, GL_STATIC_DRAW);
	m_gl->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QVector2D), nullptr);
	m_gl->glEnableVertexAttribArray(0);


	m_gl->glViewport(0, 0, width, height);
	m_gl->glDisable(GL_DEPTH_TEST);

	m_gl->glActiveTexture(GL_TEXTURE0);
	m_gl->glBindTexture(GL_TEXTURE_2D, imageData->texture());

	m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	program.setUniformValue("src", 0);
	setUniforms(program);

	m_gl->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	m_gl->glDeleteBuffers(1, &buffer);
	m_gl->glDeleteVertexArrays(1, &vao);
	m_gl->glDeleteFramebuffers(1, &fbo);

	program.release();
	imageData->doneContext();

	return processedTexture;
}

void Scale::setUniforms(QOpenGLShaderProgram& program)
{
	program.setUniformValue("mode", m_bilinear);
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
