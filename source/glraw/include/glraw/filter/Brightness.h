#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

class GLRAW_API Brightness : public AbstractFilter
{
public:
	Brightness(float size);
	Brightness(const QVariantMap& cfg);
	virtual ~Brightness() = default;

protected:
	virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass) override;
	virtual QString fragmentShaderSource(unsigned int pass) override;

private:
	float m_amount;
	float Brightness::AmountFromVariant(const QVariantMap& cfg, float default_value);
};

}