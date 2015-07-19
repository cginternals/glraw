#include <glraw/filter/GaussianBlur.h>

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
			uniform float[101] kernel;

			in vec2 v_uv;
			out vec4 dst;

			void main()
			{
				
				float offset = 1.0 / textureSize(src, 0).y;
				vec3 color = vec3(0.f);
				
				for (int i = -size; i <= size; ++i)
				{
					color += kernel[i+size]*texture(src, v_uv + vec2(0.0, i * offset)).rgb;
				}
				dst = vec4(color, texture(src, v_uv).a);
			})";

	const char* horizontalShader =
		R"(#version 150

			uniform sampler2D buf;
			uniform int size;
			uniform float[101] kernel;

			in vec2 v_uv;
			out vec4 dst;

			void main()
			{
				float offset = 1.0 / textureSize(buf, 0).x;
				vec3 color = vec3(0.f);
				
				for (int i = -size; i <= size; ++i)
				{
					color += kernel[i+size]*texture(buf, v_uv + vec2(i * offset, 0.0)).rgb;
				}
				dst = vec4(color, texture(buf, v_uv).a);
			})";


	const int DefaultSize = 5;
	const float DefaultSigma = 5.f;
}

namespace glraw
{

	GaussianBlur::GaussianBlur(unsigned int size = DefaultSize, float sigma = DefaultSigma)
		: m_size(VerifySize(size))
		, m_sigma(sigma)
		, m_kernel(CalculateKernel(size))
	{
	}

	GaussianBlur::GaussianBlur(const QVariantMap& in)
		: GaussianBlur(SizeFromVariant(in, DefaultSize), SigmaFromVariant(in, DefaultSigma))
	{
	}
	unsigned int GaussianBlur::numberOfPasses()
	{
		return 2;
	}

	void GaussianBlur::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
	{
		program.setUniformValue("size", m_size);
		program.setUniformValueArray("kernel", m_kernel, (int)m_size*2 + 1, 1);
	}

	QString GaussianBlur::fragmentShaderSource(unsigned int pass)
	{
		switch (pass)
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

	float* GaussianBlur::CalculateKernel(unsigned int size)
	{
		float *toReturn = new float[size*2 + 1];

		float pi = 3.14159265358979323846f;
		float e = 2.71828182846f;

		float sum = 0.f;
		for (int i = 0; i < size*2+1;++i)
		{
			toReturn[i] = 1 / (2 * pi*m_sigma*m_sigma)*pow(e, -((i - size)*(i - size) / (2 * m_sigma*m_sigma)));
			sum += toReturn[i];
		}
		for (int i = 0; i < size * 2 + 1; ++i)
		{
			toReturn[i] /= sum;
		}

		return toReturn;
	}

	float GaussianBlur::SigmaFromVariant(const QVariantMap& cfg, float default_value)
	{
		return cfg.value("sigma", { default_value }).toFloat();
	}
}