#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

class GLRAW_API RadialBlur : public AbstractFilter
{
public:
	RadialBlur(float x, float y, float blur, float bright, unsigned int size);
	RadialBlur(const QVariantMap& cfg);
	virtual ~RadialBlur() = default;

protected:

	virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass) override;
	virtual QString fragmentShaderSource(unsigned int pass) override;

private:
	float m_x;
	float m_y;
	float m_blur;
	float m_bright;
	unsigned int m_size; 

	float XFromVariant(const QVariantMap& cfg, float default_value);
	float YFromVariant(const QVariantMap& cfg, float default_value);
	float BlurFromVariant(const QVariantMap& cfg, float default_value);
	float BrightFromVariant(const QVariantMap& cfg, float default_value);
};

}