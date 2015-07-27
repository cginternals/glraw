#include <glraw/filter/UnsharpMask.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <QOpenGLShaderProgram>

namespace
{
	const char * const firstPass =
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
					color += kernel[abs(i)]*texture(src, v_uv + vec2(0.0, i * offset)).rgb;
				}
				dst = vec4(color, texture(src, v_uv).a);
			})";

	const char * const secondPass =
		R"(#version 150

			uniform sampler2D src;
			uniform sampler2D buf;
			uniform int size;
			uniform float factor;
			uniform float threshold;
			uniform float[101] kernel;

			in vec2 v_uv;
			out vec4 dst;

			void main()
			{
				//Standard blurring
				float offset = 1.0 / textureSize(buf, 0).x;
				vec3 blurColor = vec3(0.f);				

				for (int i = -size; i <= size; ++i)
				{
					blurColor += kernel[abs(i)]*texture(buf, v_uv + vec2(i * offset, 0.0)).rgb;
				}
				
				//unsharp mask
				dst = texture(src, v_uv);
				float factorN = clamp(factor,0.f,1.f);
				
				vec3 diff = clamp(dst.rgb - blurColor, vec3(0.f), vec3(1.f));
				diff = clamp(2*factorN*diff, vec3(0.f), vec3(1.f));
				if(diff.r*0.2126f+diff.g*0.7152f+diff.b*0.0722f > threshold)
				{
					dst += vec4(diff,0.f);
				}
				dst = clamp(dst, vec4(0.f), vec4(1.f));
			})";

	const unsigned int DefaultSize = 1;
	const float DefaultFactor = 1.f;
	const float DefaultThreshold = 0.f;
}

namespace glraw
{

UnsharpMask::UnsharpMask(unsigned int size = DefaultSize, float factor = DefaultFactor, float threshold = DefaultThreshold)
	: m_size(VerifySize(size))
	, m_factor(factor)
	, m_threshold(threshold)
	, m_kernel(CalculateKernel(size))
{
}

UnsharpMask::UnsharpMask(const QVariantMap& cfg)
	: UnsharpMask(GetSize(DefaultSize, cfg), GetFactor(DefaultFactor, cfg), Get("threshold", DefaultThreshold, cfg))
{
}

void UnsharpMask::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
{
	program.setUniformValue("size", m_size);
	program.setUniformValueArray("kernel", m_kernel, (int)m_size + 1, 1);
	if (pass == Pass::Second)
	{
		program.setUniformValue("factor", m_factor);
		program.setUniformValue("threshold", m_threshold);
	}
}

unsigned int UnsharpMask::numberOfPasses()
{
	return 2;
}

QString UnsharpMask::fragmentShaderSource(unsigned int pass)
{
	switch (pass)
	{
	case Pass::First:
		return firstPass;

	case Pass::Second:
		return secondPass;

	default:
		qCritical("Invalid pass used");
		return nullptr;
	}
}

float* UnsharpMask::CalculateKernel(unsigned int size)
{
	float *toReturn = new float[size + 1];

	float sigma = 5.f;

	float sum = 0.f;
	for (int i = 0; i < size + 1; ++i)
	{
		toReturn[i] = 1 / (2 * M_PI*sigma*sigma)*pow(M_E, -((i - size)*(i - size) / (2 * sigma*sigma)));
		sum += toReturn[i];
	}
	for (int i = 0; i < size + 1; ++i)
	{
		toReturn[i] /= sum;
	}

	return toReturn;
}

}
