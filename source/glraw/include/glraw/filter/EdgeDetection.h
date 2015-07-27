#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

class GLRAW_API EdgeDetection : public AbstractFilter
{
public:
	EdgeDetection(unsigned int size);
	EdgeDetection(const QVariantMap& cfg);
	virtual ~EdgeDetection() = default;

protected:

	virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass) override;
	virtual QString fragmentShaderSource(unsigned int pass) override;

private:

	unsigned int m_size;
};

}