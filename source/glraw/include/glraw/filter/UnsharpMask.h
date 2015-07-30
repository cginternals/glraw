#pragma once
#include <glraw/filter/GaussianBlur.h>

namespace glraw
{

class GLRAW_API UnsharpMask : public GaussianBlur
{
public:
	UnsharpMask(unsigned int size, float factor, float threshold, float sigma);
	UnsharpMask(const QVariantMap& cfg);
	virtual ~UnsharpMask() = default;

protected:
	virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass) override;
	virtual QString firstShader() const override;
	virtual QString secondShader() const override;

	float m_threshold;
};

}