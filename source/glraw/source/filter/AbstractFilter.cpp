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
			gl_Position = vec4(a_vertex * 1.0, 0.0, 1.0);
		}
		)";
}

namespace glraw
{

bool AbstractFilter::renderShader(std::unique_ptr<Canvas> & imageData, const QString & shader)
{
	GLuint processedTexture = renderShaderToTexture(imageData, shader);

	if( processedTexture == 0 )
	{
		qCritical("Error rendering to texture!");
		return false;
	}

	imageData->updateTexture(processedTexture);

	return true;
}

int AbstractFilter::renderShaderToTexture( std::unique_ptr<Canvas> & imageData, const QString & shader )
{
	imageData->makeContext();
	QOpenGLShaderProgram program;
	if( !createProgram( program, shader ) )
	{
		qCritical( "Shader Error!" );
		return 0;
	}
	GLuint texture = imageData->texture();
	auto m_gl = imageData->gl();

	GLint width, height;
	m_gl->glBindTexture( GL_TEXTURE_2D, texture );
	m_gl->glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width );
	m_gl->glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height );

	GLuint processedTexture;
	m_gl->glGenTextures( 1, &processedTexture );
	m_gl->glBindTexture( GL_TEXTURE_2D, processedTexture );
	m_gl->glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr );

	GLuint fbo;
	m_gl->glGenFramebuffers( 1, &fbo );
	m_gl->glBindFramebuffer( GL_FRAMEBUFFER, fbo );
	m_gl->glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, processedTexture, 0 );

	GLuint vao;
	m_gl->glGenVertexArrays( 1, &vao );
	m_gl->glBindVertexArray( vao );

	static const float rawv[] = { +1.f, -1.f, +1.f, +1.f, -1.f, -1.f, -1.f, +1.f };

	GLuint buffer;
	m_gl->glGenBuffers( 1, &buffer );
	m_gl->glBindBuffer( GL_ARRAY_BUFFER, buffer );
	m_gl->glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 8, rawv, GL_STATIC_DRAW );
	m_gl->glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof( QVector2D ), nullptr );
	m_gl->glEnableVertexAttribArray( 0 );


	m_gl->glViewport( 0, 0, width, height );
	m_gl->glDisable( GL_DEPTH_TEST );

	m_gl->glActiveTexture( GL_TEXTURE0 );
	m_gl->glBindTexture( GL_TEXTURE_2D, imageData->texture() );

	m_gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	m_gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	m_gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	m_gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	program.setUniformValue( "src", 0 );
	setUniforms(program);

	m_gl->glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

	m_gl->glDeleteBuffers( 1, &buffer );
	m_gl->glDeleteVertexArrays( 1, &vao );
	m_gl->glDeleteFramebuffers( 1, &fbo );

	program.release();
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

void AbstractFilter::setUniforms(QOpenGLShaderProgram& program)
{
}

unsigned int AbstractFilter::numberOfPasses()
{
	return 1;
}

unsigned int AbstractFilter::VerifySize( unsigned int size )
{
	if( size == 0 )
	{
		qDebug( "The minimum size is 1." );
		return 1;
	}
	else
	{
		return size;
	}
}

unsigned int AbstractFilter::SizeFromVariant( const QVariantMap& cfg, unsigned int default_value )
{
	return cfg.value( "size", { default_value } ).toInt();
}

float AbstractFilter::FactorFromVariant( const QVariantMap& cfg, float default_value )
{
	return cfg.value( "factor", { default_value } ).toFloat();
}
}