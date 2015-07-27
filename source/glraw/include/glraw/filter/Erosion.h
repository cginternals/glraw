#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{
class GLRAW_API Erosion : public AbstractFilter
{
public:
	Erosion(unsigned int size);
	Erosion(const QVariantMap& cfg);
	virtual ~Erosion() = default;

protected:
	virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass) override;
	virtual QString fragmentShaderSource(unsigned int pass) override;

	unsigned int m_size;
};

}