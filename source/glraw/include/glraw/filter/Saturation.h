#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

class GLRAW_API Saturation : public AbstractFilter
{
public:
	Saturation(float size);
	Saturation(const QVariantMap& cfg);
	virtual ~Saturation() = default;

protected:
	virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass) override;
	virtual QString fragmentShaderSource(unsigned int pass) override;

	float m_amount;
};

}