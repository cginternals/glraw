#include <glraw/filter/Scale.h>

#include <QOpenGLShaderProgram>

#include <glraw/Canvas.h>

namespace
{
	const char * const source =
		R"(#version 150

		uniform sampler2D src;
		uniform int width;
		uniform int height;
		uniform bool mode;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			ivec2 size = textureSize(source, 0);
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

		} )";

	const int DefaultWidth = 500;
	const int DefaultHeight = 500;
	bool DefaultFastMode = true;
}

namespace glraw
{

Scale::Scale( int width = DefaultWidth, int height = DefaultHeight, bool fastMode = DefaultFastMode )
	: m_width(height)
	, m_height(height)
	, m_fastMode(fastMode)
{
}

Scale::Scale(const QVariantMap& cfg)
	: Scale(WidthFromVariant(cfg), HeightFromVariant(cfg), FastModeFromVariant(cfg))
{

}

bool Scale::process(std::unique_ptr<Canvas> & imageData, AssetInformation & info)
{
	imageData->makeContext();
	QOpenGLShaderProgram program;
	if (!createProgram(program, source))
	{
		qCritical("Shader Error!");
		return 0;
	}
	GLuint texture = imageData->texture();
	auto m_gl = imageData->gl();

	m_gl->glBindTexture(GL_TEXTURE_2D, texture);

	GLuint processedTexture;
	m_gl->glGenTextures(1, &processedTexture);
	m_gl->glBindTexture(GL_TEXTURE_2D, processedTexture);
	m_gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);

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


	m_gl->glViewport(0, 0, m_width, m_height);
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
	program.setUniformValue( "height", m_height );
	program.setUniformValue("width", m_height);
	program.setUniformValue("mode", m_fastMode);
}

int Scale::WidthFromVariant(const QVariantMap& cfg)
{
	return cfg.value( "width", { DefaultWidth } ).toInt();
}
int Scale::HeightFromVariant(const QVariantMap& cfg)
{
	return cfg.value("height", { DefaultHeight }).toInt();
}
bool Scale::FastModeFromVariant(const QVariantMap& cfg)
{
	return cfg.value("fastMode", { DefaultFastMode }).toBool();
}

}