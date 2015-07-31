#include <glraw/filter/GaussianBlur.h>

#include <cassert>
#define _USE_MATH_DEFINES
#include <math.h>

#include <QOpenGLShaderProgram>


namespace
{
	const char* verticalShader =
		R"(#version 150

			uniform sampler2D src;
			uniform int size;
			uniform float[101] kernel;

			in vec2 v_uv;
			out vec4 dst;

			void main()
			{
				
				float offset = 1.0 / textureSize(src, 0).y;
				
				for (int i = -size; i <= size; ++i)
				{
					dst += kernel[abs(i)] * texture(src, v_uv + vec2(0.0, i * offset));
				}
			})";

	const char* horizontalShader =
		R"(#version 150

			uniform sampler2D src;
			uniform sampler2D buf;
			uniform int size;
			uniform float factor;
			uniform float[101] kernel;

			in vec2 v_uv;
			out vec4 dst;

			void main()
			{
				float offset = 1.0 / textureSize(buf, 0).x;
				dst = vec4(0.f);
				
				for (int i = -size; i <= size; ++i)
				{
					dst += kernel[abs(i)]*texture(buf, v_uv + vec2(i * offset, 0.0));
				}
				vec4 texel = texture(src, v_uv);
				dst = vec4(mix(texel, dst, factor).rgb, texel.a);
			})";


	const int DefaultSize = 5;
	const float DefaultFactor = 1.0f;
	const float DefaultSigma = 10.0f;
}

namespace glraw
{

GaussianBlur::GaussianBlur(unsigned int size = DefaultSize, float factor = DefaultFactor, float sigma = DefaultSigma)
	: AbstractKernel(size, factor)
	, m_kernel(CalculateKernel(m_size, sigma))
{
}

GaussianBlur::GaussianBlur(const QVariantMap& in)
	: GaussianBlur(GetSize(DefaultSize, in),GetFactor(DefaultFactor, in), Get("sigma", DefaultSigma, in))
{
}

GaussianBlur::~GaussianBlur() 
{ 
	delete[] m_kernel; 
}

void GaussianBlur::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
{
	AbstractKernel::setUniforms(program, pass);
	program.setUniformValueArray("kernel", m_kernel, static_cast<int>(m_size + 1), 1);
}

QString GaussianBlur::firstShader() const
{
	return verticalShader;
}

QString GaussianBlur::secondShader() const
{
	return horizontalShader;
}

float* GaussianBlur::CalculateKernel(int size, float sigma)
{
	float *toReturn = new float[size + 1];

	float sum = 0.0f;
	const float strength = 1.0f / (2.0f*sigma*sigma);
	for (int i = 0; i < size+1;++i)
	{
		const int pos = i - size;
		toReturn[i] = M_PI*strength * pow(M_E, -pos*pos*strength);
		sum += toReturn[i];
	}
	sum *= sqrt(3);
	for (int i = 0; i < size + 1; ++i)
	{
		toReturn[i] /= sum;
	}

	return toReturn;
}

}