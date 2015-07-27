#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{
	class GLRAW_API UnsharpMask : public AbstractFilter
	{
	public:
		UnsharpMask(unsigned int size, float factor, float threshold);
		UnsharpMask(const QVariantMap& cfg);
		virtual ~UnsharpMask() { delete[] m_kernel; };

	protected:
		virtual unsigned int numberOfPasses() override;
		virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass) override;
		virtual QString fragmentShaderSource(unsigned int pass) override;

	private:
		unsigned int m_size;
		float m_factor;
		float m_threshold;
		float *m_kernel;

		float* CalculateKernel(unsigned int size);
	};

}