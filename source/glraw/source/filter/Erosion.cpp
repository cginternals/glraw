#include <glraw/filter/Erosion.h>

#include <QOpenGLShaderProgram>

namespace
{
	const char * const source =
		R"(#version 150

		uniform sampler2D src;
		uniform int size;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			vec2 img_size = vec2(1.0f)/textureSize(src, 0);
			dst = vec4(1.0f);

			for(int i=-size; i<=size;++i)
			{
				for(int j=-size; j <=size; ++j)
				{
					dst = min(dst, texture(src, v_uv+img_size*vec2(i,j)));
				}
			}
		} )";

	const unsigned int DefaultSize = 1;
}

namespace glraw
{

Erosion::Erosion(unsigned int size = DefaultSize)
	: m_size(VerifySize(size))
{
}

Erosion::Erosion(const QVariantMap& cfg)
	: Erosion(GetSize(DefaultSize, cfg))
{
}

void Erosion::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
{
	program.setUniformValue("size", m_size);
}

QString Erosion::fragmentShaderSource(unsigned int pass)
{
	return source;
}

}
