#pragma once
#include <glraw/filter/AbstractFilter.h>

#include <QVector2D>

namespace glraw
{

class GLRAW_API RadialBlur : public AbstractFilter
{
public:
	RadialBlur(float x, float y, float blur, unsigned int size);
	RadialBlur(const QVariantMap& cfg);
	virtual ~RadialBlur() = default;

protected:
	virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass) override;
	virtual QString fragmentShaderSource(unsigned int pass) override;

	QVector2D m_position;
	float m_blur;
	unsigned int m_size; 
};

}