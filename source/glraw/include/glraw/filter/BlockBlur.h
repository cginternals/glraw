#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

class GLRAW_API BlockBlur : public AbstractFilter
{
public:
	BlockBlur(unsigned int size, float factor);
	BlockBlur(const QVariantMap&);
	virtual ~BlockBlur() = default;

protected:

	virtual unsigned int numberOfPasses() override;
	virtual void setUniforms( QOpenGLShaderProgram& program, unsigned int pass ) override;
	virtual QString fragmentShaderSource(unsigned int pass) override;

private:

	unsigned int m_size;
	float m_factor;
};

}