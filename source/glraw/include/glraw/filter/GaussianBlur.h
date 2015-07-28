#pragma once
#include <glraw/filter/AbstractKernel.h>

namespace glraw
{

class GLRAW_API GaussianBlur : public AbstractKernel
{
public:
	GaussianBlur(unsigned int size, float factor, float sigma);
	GaussianBlur(const QVariantMap&);
	virtual ~GaussianBlur() { delete [] m_kernel; };

protected:
	virtual void setUniforms( QOpenGLShaderProgram& program, unsigned int pass ) override;
	virtual QString firstShader() const override;
	virtual QString secondShader() const override;

	float *m_kernel;

	static float* CalculateKernel(unsigned int size, float sigma);
};

}