#include <glraw/filter/Rotate.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLShaderProgram>

#include <glraw/AssetInformation.h>

namespace
{
	const char * const source =
		R"(#version 150

		uniform sampler2D src;
		uniform int rotation;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			float sin = (rotation==1)?1.f:(rotation==3)?-1.f:0.f;
			float cos = (rotation==0)?1.f:(rotation==2)?-1.f:0.f;
			vec2 texCoords = vec2((v_uv.x-0.5f)*cos - (v_uv.y-0.5f)*sin + 0.5f, (v_uv.x-0.5f)*sin + (v_uv.y-0.5f)*cos + 0.5f);
			dst = texture(src, texCoords);
		} )";

	const unsigned int DefaultRotation = static_cast<unsigned int>(glraw::Rotate::RotationMode::Default);
}

namespace glraw
{

Rotate::Rotate(unsigned int rotation = DefaultRotation)
	: m_rotation(rotation%4)
{
}

Rotate::Rotate(const QVariantMap& cfg)
	: Rotate(Get("rotation", DefaultRotation, cfg))
{
}

void Rotate::updateAssetInformation(AssetInformation & info)
{
	info.setProperty("width", out_width);
	info.setProperty("height", out_height);
}

int Rotate::createWorkingTexture(unsigned int prototype)
{
	GLint width, height;
	m_gl->glBindTexture(GL_TEXTURE_2D, prototype);
	m_gl->glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	m_gl->glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

	GLuint buffer_texture;
	m_gl->glGenTextures(1, &buffer_texture);
	m_gl->glBindTexture(GL_TEXTURE_2D, buffer_texture);


	if (m_rotation == 1 || m_rotation == 3)
	{
		out_width = height;
		out_height = width;
	}
	else
	{
		out_width = width;
		out_height = height;
	}

	m_gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, out_width, out_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	m_gl->glViewport(0, 0, out_width, out_height);

	return buffer_texture;
}

void Rotate::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
{
	program.setUniformValue( "rotation", m_rotation);
}

QString Rotate::fragmentShaderSource(unsigned int pass)
{
	return source;
}

}