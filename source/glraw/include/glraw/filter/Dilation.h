#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

class GLRAW_API Dilation : public AbstractFilter
{
public:
	Dilation(unsigned int size);
	Dilation(const QVariantMap& cfg);
	virtual ~Dilation() = default;

protected:
	virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass) override;
	virtual QString fragmentShaderSource(unsigned int pass) override;

	unsigned int m_size;
};

}