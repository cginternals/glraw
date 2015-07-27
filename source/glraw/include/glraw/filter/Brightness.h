#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

class GLRAW_API Brightness : public AbstractFilter
{
public:
	Brightness(float amount);
	Brightness(const QVariantMap& cfg);
	virtual ~Brightness() = default;

protected:
	virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass) override;
	virtual QString fragmentShaderSource(unsigned int pass) override;

	float m_amount;
};

}