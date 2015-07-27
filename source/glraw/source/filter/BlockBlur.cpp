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
			uniform int size;

			in vec2 v_uv;
			out vec4 fragColor;

			void main()
			{
				vec3 color = vec3(0.0);
				float offset = 1.0 / textureSize(src, 0).y;
				for (int i = -size; i <= size; ++i)
				{
					color += texture(src, v_uv + vec2(0.0, i * offset)).xyz;
				}
				fragColor = vec4(color / (2*size+1), 1.0);
			})";

	const char* horizontalShader =
		R"(#version 150

			uniform sampler2D src;
			uniform sampler2D buf;
			uniform float factor;
			uniform int size;

			in vec2 v_uv;
			out vec4 fragColor;

			void main()
			{
				vec3 color = vec3(0.0);
				float offset = 1.0 / textureSize(src, 0).x;
				for (int i = -size; i <= size; ++i)
				{
					color += texture(buf, v_uv + vec2(i * offset, 0.0)).xyz;
				}
				color = mix(texture(src, v_uv).xyz, color / (2*size+1), factor);
				fragColor = vec4(color, 1.0);
			})";


	const int DefaultSize = 1;
	const float DefaultFactor = 1.0f;
}

namespace glraw
{

BlockBlur::BlockBlur( unsigned int size = DefaultSize, float factor = DefaultFactor )
	: m_size(VerifySize(size))
	, m_factor(factor)
{
}

BlockBlur::BlockBlur(const QVariantMap& in)
	: BlockBlur(GetSize(DefaultSize, in), GetFactor(DefaultFactor, in))
{
}

unsigned int BlockBlur::numberOfPasses()
{
	return 2;
}

void BlockBlur::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
{
	program.setUniformValue( "size", m_size );
	if(pass == Pass::Second)
	{
		program.setUniformValue("factor", m_factor);
	}
}

QString BlockBlur::fragmentShaderSource(unsigned int pass)
{
	switch(pass)
	{
	case Pass::First:
		return verticalShader;

	case Pass::Second:
		return horizontalShader;

	default:
		qCritical("Invalid pass used");
		return nullptr;
	}
}

}