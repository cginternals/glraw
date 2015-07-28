#include <glraw/filter/AbstractFilter.h>

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_2_Core>

#include <glraw/Canvas.h>

namespace
{
	const char * vertexShaderSource =
		R"(#version 150

		in vec2 a_vertex;
		out vec2 v_uv;

		void main()
		{
			v_uv = a_vertex.xy * 0.5 + 0.5;    
			gl_Position = vec4(a_vertex, 0.0, 1.0);
		}
		)";
}

namespace glraw
{

AbstractFilter::AbstractFilter()
	: m_gl(nullptr)
{
}

bool AbstractFilter::process(std::unique_ptr<Canvas> & imageData, AssetInformation & info)
{
	m_gl = imageData->gl();
	GLuint processedTexture = renderToTexture(imageData);

	if(processedTexture == 0)
	{
		qCritical("Error rendering to texture!");
		return false;
	}

	if(numberOfPasses() % 2 != 0)
	{
		imageData->updateTexture(processedTexture);
	}

	updateAssetInformation(info);

	return true;
}

void AbstractFilter::updateAssetInformation(AssetInformation & info)
{
}

int AbstractFilter::renderToTexture(std::unique_ptr<Canvas> & imageData)
{
	imageData->makeContext();

	GLuint processedTexture = createWorkingTexture(imageData->texture());

	for(unsigned int i = 0; i < numberOfPasses(); i++)
	{
		QOpenGLShaderProgram program;
		if(!createProgram(program, fragmentShaderSource(i)))
		{
			qCritical("Shader Error!");
			return 0;
		}

		if(i % 2)
		{
			attachToFramebuffer(imageData->texture());
		}
		else
		{
			attachToFramebuffer(processedTexture);
		}
		bindVertexBuffer();

		bindTexture(0, imageData->texture());
		program.setUniformValue("src", 0);
		if(i > 0)
		{
			bindTexture(1, processedTexture);
			program.setUniformValue("buf", 1);
		}

		setUniforms(program, i);

		m_gl->glDrawArrays(GL_TRIANGLES, 0, 3);

		program.release();
	}

	imageData->doneContext();

	return processedTexture;
}

bool AbstractFilter::createProgram(QOpenGLShaderProgram& program, const QString & shader)
{
	program.bindAttributeLocation("a_vertex", 0);
	program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);

	if (!program.addShaderFromSourceCode(QOpenGLShader::Fragment, shader))
	{
		qCritical() << program.log();
		return false;
	}

	if (!program.link())
	{
		qCritical() << program.log();
		return false;
	}

	return program.bind();
}

void AbstractFilter::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
{
}

unsigned int AbstractFilter::numberOfPasses()
{
	return 1;
}

void AbstractFilter::bindTexture(unsigned int unit, unsigned int tex)
{
	m_gl->glActiveTexture(GL_TEXTURE0 + unit);
	m_gl->glBindTexture(GL_TEXTURE_2D, tex);

	m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

int AbstractFilter::createWorkingTexture(unsigned int prototype)
{
	GLint width, height;
	m_gl->glBindTexture(GL_TEXTURE_2D, prototype);
	m_gl->glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	m_gl->glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

	GLuint buffer_texture;
	m_gl->glGenTextures(1, &buffer_texture);
	m_gl->glBindTexture(GL_TEXTURE_2D, buffer_texture);
	m_gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

	m_gl->glViewport(0, 0, width, height);

	return buffer_texture;
}

void AbstractFilter::attachToFramebuffer(unsigned int texture)
{
	static bool initialized = false;
	static GLuint fbo;

	if(!initialized)
	{
		m_gl->glGenFramebuffers(1, &fbo);
		m_gl->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		initialized = true;
	}

	m_gl->glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
}

void AbstractFilter::bindVertexBuffer()
{
	// Screen aligned triangle.
	static const float rawv[] = { +1.f, -1.f, +1.f, +3.f, -3.f, -1.f };
	static bool initialized = false;
	static GLuint vao, buffer;

	if(!initialized)
	{
		m_gl->glGenVertexArrays(1, &vao);
		m_gl->glBindVertexArray(vao);

		m_gl->glGenBuffers(1, &buffer);
		m_gl->glBindBuffer(GL_ARRAY_BUFFER, buffer);

		m_gl->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, rawv, GL_STATIC_DRAW);
		m_gl->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QVector2D), nullptr);
		m_gl->glEnableVertexAttribArray(0);
		initialized = true;
	}
	else
	{
		m_gl->glBindVertexArray(vao);
		m_gl->glBindBuffer(GL_ARRAY_BUFFER, buffer);
	}
}

}