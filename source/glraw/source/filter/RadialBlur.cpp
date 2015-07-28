#include <glraw/filter/RadialBlur.h>

#include <QOpenGLShaderProgram>

namespace
{
	const char * const source =
		R"(#version 150

		uniform sampler2D src;
		uniform vec2 pos;
		uniform float blur;
		uniform int size;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			vec2 img_size = vec2(1.0f)/textureSize(src, 0);
			vec3 color;
			vec2 coords = v_uv+img_size*0.5-pos;
			
			for(int i=0;i<size;++i)
			{
				float scale = 1.0f - blur*i/(size-1.0);
				color += texture(src, coords*scale + pos).rgb;
			}
			dst = vec4(color/size, texture(src,v_uv).a);
		} )";

	const float DefaultX = 0.5f;
	const float DefaultY = 0.5f;
	const float DefaultBlur = 0.1f;
	const int DefaultSize = 10;
}

namespace glraw
{

RadialBlur::RadialBlur(float x = DefaultX, float y = DefaultY, float blur = DefaultBlur, unsigned int size = DefaultSize)
	: m_position(x, y)
	, m_blur(blur)
	, m_size(VerifySize(size))
{
}

RadialBlur::RadialBlur(const QVariantMap& cfg)
	: RadialBlur(
		Get("x", DefaultX, cfg), 
		Get("y", DefaultY, cfg),
		Get("blur", DefaultBlur, cfg), 
		GetSize(DefaultSize, cfg))
{
}

void RadialBlur::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
{
	program.setUniformValue("pos", m_position);
	program.setUniformValue("blur", m_blur);
	program.setUniformValue("size", m_size);
}

QString RadialBlur::fragmentShaderSource(unsigned int pass)
{
	return source;
}

}
