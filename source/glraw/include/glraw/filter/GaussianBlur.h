#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

class GLRAW_API GaussianBlur : public AbstractFilter
{
public:
	GaussianBlur(unsigned int size, float sigma);
	GaussianBlur(const QVariantMap&);
	virtual ~GaussianBlur() { delete [] m_kernel; };

protected:
	virtual unsigned int numberOfPasses() override;
	virtual void setUniforms( QOpenGLShaderProgram& program, unsigned int pass ) override;
	virtual QString fragmentShaderSource(unsigned int pass) override;

	unsigned int m_size;
	float m_sigma;
	float *m_kernel;

	float* CalculateKernel(unsigned int size);
};

}