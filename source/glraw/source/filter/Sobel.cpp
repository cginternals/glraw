#include <glraw/filter/Sobel.h>

#include <QOpenGLShaderProgram>

namespace
{
	const char * const source =
		R"(#version 150

			uniform sampler2D src;
			uniform vec3 factor;
			uniform float[18] kernel;

			in vec2 v_uv;
			out vec4 dst;

			void main()
			{   
				vec2 offset = 1.0f / textureSize(src, 0);

				vec4 Gx = vec4(0.f);
				vec4 Gy = vec4(0.f);
				vec3 color = vec3(0.f);
				for(int i=-1;i<=1;++i)
				{
					for(int j=-1;j<=1;++j)
					{
						vec4 color = texture(src,v_uv+offset*vec2(i,j));
						Gx += kernel[3*j+i+4]*color;
						Gy += kernel[3*j+i+13]*color;
					}
				}
				dst = abs(Gx)+abs(Gy);
			} )";
}

namespace glraw
{

	Sobel::Sobel(SobelMode in = SobelMode::Default)
		: m_mode(in)
{
}

Sobel::Sobel(const QVariantMap& cfg)
	: Sobel(ModeFromVariant(cfg))
{
}

void Sobel::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
{
	float sobel[18] = { -1.f, -2.f, -1.f, 0.f, 0.f, 0.f, 1.f, 2.f, 1.f, -1.f, 0.f, 1.f, -2.f, 0.f, 2.f, -1.f, 0.f, 1.f };
	float scharr[18] = { 3.f, 10.f, 3.f, 0.f, 0.f, 0.f, -3.f, -10.f, -3.f, 3.f, 0.f, -3.f, 10.f, 0.f, -10.f, 3.f, 0.f, -3.f };
	switch (m_mode)
	{
	case SobelMode::Sobel:
		program.setUniformValueArray("kernel", sobel, 18, 1);
			break;
	case SobelMode::Scharr:
		program.setUniformValueArray("kernel", scharr, 18, 1);
			break;
	}
}

QString Sobel::fragmentShaderSource(unsigned int pass)
{
	return source;
}

SobelMode Sobel::ModeFromVariant(const QVariantMap& cfg)
{
	return static_cast<SobelMode>(cfg.value("mode", { static_cast<int>(SobelMode::Default) }).toInt());
}

}
