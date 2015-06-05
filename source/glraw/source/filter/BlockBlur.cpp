#include <glraw/filter/BlockBlur.h>

#include <cassert>

#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLShaderProgram>

#include <glraw/Canvas.h>

namespace
{
const char* verticalShader = 
		R"(#version 150

		uniform sampler2D src;
		uniform int range;

		in vec2 v_uv;
		out vec4 fragColor;

		void main()
		{
			vec3 color = vec3(0.0);
			float offset = 1.0 / textureSize(src, 0).y;
			for (int i = -range; i <= range; ++i)
			{
				color += texture(src, v_uv + vec2(0.0, i * offset)).xyz;
			}
			fragColor = vec4(color / (2*range+1), 1.0);
		})";

const char* horizontalShader =
		R"(#version 150

		uniform sampler2D src;
		uniform sampler2D buf;
		uniform float factor;
		uniform int range;

		in vec2 v_uv;
		out vec4 fragColor;

		void main()
		{
			vec3 color = vec3(0.0);
			float offset = 1.0 / textureSize(src, 0).x;
			for (int i = -range; i <= range; ++i)
			{
				color += texture(buf, v_uv + vec2(i * offset, 0.0)).xyz;
			}
			color = (color / (2*range+1)) * factor + texture(src, v_uv).xyz * (1.0-factor);
			fragColor = vec4(color, 1.0);
		})";


	const int DefaultRange = 1;
	const float DefaultFactor = 1.0f;
}

namespace glraw
{

BlockBlur::BlockBlur( unsigned int range = DefaultRange, float factor = DefaultFactor )
	: m_range(VerifyRange(range))
	, m_factor(factor)
{
}

BlockBlur::BlockBlur(const QVariantMap& in)
	: BlockBlur( RangeFromVariant(in), FactorFromVariant(in) )
{
}

void BlockBlur::setUniforms( QOpenGLShaderProgram& program )
{
	program.setUniformValue( "range", m_range );
}

bool BlockBlur::process(std::unique_ptr<Canvas> & imageData, AssetInformation & info)
{
	//TODO improve!
	imageData->makeContext();
	QOpenGLShaderProgram program;
	if( !createProgram( program, verticalShader ) )
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

	setUniforms(program);

	m_gl->glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

	program.release();

	QOpenGLShaderProgram program2;
	if( !createProgram( program2, horizontalShader ) )
	{
		qCritical( "Shader Error!" );
		return 0;
	}
	m_gl->glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0 );

	m_gl->glActiveTexture( GL_TEXTURE1 );
	m_gl->glBindTexture( GL_TEXTURE_2D, processedTexture );

	m_gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	m_gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	m_gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	m_gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	setUniforms(program2);
	program2.setUniformValue("buf", 1);
	program2.setUniformValue( "factor", m_factor );

	m_gl->glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

	m_gl->glDeleteBuffers( 1, &buffer );
	m_gl->glDeleteVertexArrays( 1, &vao );
	m_gl->glDeleteFramebuffers( 1, &fbo );
	m_gl->glDeleteTextures( 1, &processedTexture );

	imageData->doneContext();

	return true;
}

unsigned int BlockBlur::VerifyRange( unsigned int range )
{
	if( range == 0 )
	{
		qWarning( "The minimum range is 1." );
		return 1;
	}
	else
	{
		return range;
	}
}

unsigned int BlockBlur::RangeFromVariant( const QVariantMap& cfg )
{
	return cfg.value( "range", { DefaultRange } ).toInt();
}

float BlockBlur::FactorFromVariant( const QVariantMap& cfg )
{
	return cfg.value( "factor", { DefaultFactor } ).toFloat();
}

}