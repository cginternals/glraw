#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

class GLRAW_API AbstractKernel : public AbstractFilter
{
public:
	AbstractKernel(unsigned int size, float factor);
	virtual ~AbstractKernel() = default;

protected:
	virtual unsigned int numberOfPasses() override final;
	virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass) override;
	virtual QString fragmentShaderSource(unsigned int pass) override final;

	virtual QString firstShader() const = 0;
	virtual QString secondShader() const = 0;

	unsigned int m_size;
	float m_factor;
};

}