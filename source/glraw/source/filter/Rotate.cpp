#include <glraw/filter/Rotate.h>

#define _USE_MATH_DEFINES

#include <math.h>
#include <QOpenGLShaderProgram>

namespace
{
	const char * const source =
		R"(#version 150

		uniform sampler2D src;
		uniform float rotationSin;
		uniform float rotationCos;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			if(rotationSin == 0)
			{
				dst = texture(src, v_uv);
				return;
			}
			vec2 texCoords = vec2((v_uv.x-0.5f)*rotationCos - (v_uv.y-0.5f)*rotationSin + 0.5f, (v_uv.x-0.5f)*rotationSin + (v_uv.y-0.5f)*rotationCos + 0.5f);
			dst = texture(src, texCoords);
		} )";

	const int DefaultRotation = 0;
}

namespace glraw
{

Rotate::Rotate(int rotation = DefaultRotation)
	: m_rotation(rotation%4)
{
}

Rotate::Rotate(const QVariantMap& cfg)
	: Rotate(RotationFromVariant(cfg))
{

}

bool Rotate::process(std::unique_ptr<Canvas> & imageData, AssetInformation & info)
{
	return renderShader( imageData, source );
}

void Rotate::setUniforms(QOpenGLShaderProgram& program)
{
	program.setUniformValue( "rotationSin", sinf(M_PI * (float)m_rotation / 90.0f ));
	program.setUniformValue("rotationCos", sinf(M_PI * (float)m_rotation / 90.0f));
}

float Rotate::RotationFromVariant(const QVariantMap& cfg)
{
	return cfg.value("rotation", { DefaultRotation }).toInt();
}

}