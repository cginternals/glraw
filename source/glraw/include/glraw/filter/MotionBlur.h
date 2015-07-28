#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

class GLRAW_API MotionBlur : public AbstractFilter
{
public:
	MotionBlur(unsigned int size, float angle, float factor);
	MotionBlur(const QVariantMap& cfg);
	virtual ~MotionBlur() = default;

protected:
	virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass) override;
	virtual QString fragmentShaderSource(unsigned int pass) override;

	unsigned int m_size;
	float m_angle;
	float m_factor;

	float GetAngle(const QVariantMap & cfg);
};

}