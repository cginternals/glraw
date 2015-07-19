#include <glraw/filter/EdgeDetection.h>

#include <QOpenGLShaderProgram>

#include <glraw/Canvas.h>

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
			vec3 sum = texture(src, v_uv).rgb*(2*size+1)*(2*size+1);

			for(int i=-size; i<=size;++i)
			{
				for(int j=-size; j <=size; ++j)
				{
					sum -= texture(src, v_uv + img_size*vec2(i,j)).rgb;
				}
			}
			dst = vec4(sum, texture(src, v_uv).a);
		} )";

	const unsigned int DefaultSize = 1;
}

namespace glraw
{

	EdgeDetection::EdgeDetection(unsigned int size = DefaultSize)
		: m_size(VerifySize(size))
	{
	}

	EdgeDetection::EdgeDetection(const QVariantMap& cfg)
		: EdgeDetection(SizeFromVariant(cfg, DefaultSize))
	{
	}

	void EdgeDetection::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
	{
		program.setUniformValue("size", m_size);
	}

	QString EdgeDetection::fragmentShaderSource(unsigned int pass)
	{
		return source;
	}

}
