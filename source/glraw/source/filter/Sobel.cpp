#include <glraw/filter/Sobel.h>

#include <QOpenGLShaderProgram>

namespace
{
	const char * const source =
		R"(#version 150

			uniform sampler2D src;
			uniform float[18] kernel;

			in vec2 v_uv;
			out vec4 dst;

			void main()
			{   
				vec2 offset = 1.0f / textureSize(src, 0);

				vec3 Gx = vec3(0.f);
				vec3 Gy = vec3(0.f);
				for(int i=-1;i<=1;++i)
				{
					for(int j=-1;j<=1;++j)
					{
						vec3 color = texture(src,v_uv+offset*vec2(i,j)).rgb;
						Gx += kernel[3*j+i+4]*color;
						Gy += kernel[3*j+i+13]*color;
					}
				}
				dst = vec4(abs(Gx)+abs(Gy),texture(src,v_uv).a);
			} )";

	const float sobel[18] = { -1.f,	 -2.f,  -1.f, 
							   0.f,   0.f,   0.f, 
							   1.f,   2.f,   1.f,

							  -1.f,   0.f,   1.f, 
							  -2.f,   0.f,   2.f, 
							  -1.f,   0.f,   1.f };

	const float scharr[18] = { 3.f,  10.f,   3.f, 
							   0.f,   0.f,   0.f, 
							  -3.f, -10.f,  -3.f, 
							  
							   3.f,   0.f,  -3.f, 
							  10.f,   0.f, -10.f, 
							   3.f,   0.f,  -3.f };
}

namespace glraw
{

Sobel::Sobel(SobelMode in = SobelMode::Default)
	: m_kernel(GetKernel(in))
{
}

Sobel::Sobel(const QVariantMap& cfg)
	: Sobel(ModeFromVariant(cfg))
{
}

void Sobel::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
{
	program.setUniformValueArray("kernel", m_kernel, 18, 1);
}

QString Sobel::fragmentShaderSource(unsigned int pass)
{
	return source;
}

SobelMode Sobel::ModeFromVariant(const QVariantMap& cfg)
{
	return static_cast<SobelMode>(cfg.value("mode", { static_cast<int>(SobelMode::Default) }).toInt());
}

const float* Sobel::GetKernel(SobelMode mode)
{
	switch(mode)
	{
	default:
		qCritical("Invalid Sobel mode used.");

	case SobelMode::Sobel:
		return sobel;

	case SobelMode::Scharr:
		return scharr;
	}
}

}
