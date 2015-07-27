#pragma once
#include <glraw/filter/AbstractFilter.h>

#include <QVector3D>

namespace glraw
{

enum class SobelMode
{
	Sobel,
	Scharr,

	Default = Sobel
};

class GLRAW_API Sobel : public AbstractFilter
{
public:
	Sobel(SobelMode in);
	Sobel(const QVariantMap& cfg);
	virtual ~Sobel() = default;

protected:
	virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass) override;
	virtual QString fragmentShaderSource(unsigned int pass) override;

	const float* m_kernel;

	static SobelMode ModeFromVariant(const QVariantMap& cfg);
	static const float* GetKernel(SobelMode mode);
};

}