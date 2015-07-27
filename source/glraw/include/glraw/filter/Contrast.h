#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

class GLRAW_API Contrast : public AbstractFilter
{
public:
	Contrast(float factor);
	Contrast(const QVariantMap& cfg);
	virtual ~Contrast() = default;

protected:
	virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass) override;
	virtual QString fragmentShaderSource(unsigned int pass) override;

	float m_factor;
};

}