#include <glraw/filter/Rotate.h>

#define _USE_MATH_DEFINES

#include <math.h>
#include <QOpenGLShaderProgram>

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
	: Rotate(RotationFromVariant(cfg))
{
}

bool Rotate::process(std::unique_ptr<Canvas> & imageData, AssetInformation & info)
{
	return renderShader( imageData, source );
}

void Rotate::setUniforms(QOpenGLShaderProgram& program)
{
	program.setUniformValue( "rotation", m_rotation);
}

unsigned int Rotate::RotationFromVariant(const QVariantMap& cfg)
{
	return cfg.value("rotation", { DefaultRotation }).toInt();
}

}