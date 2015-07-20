#pragma once
#include <glraw/filter/AbstractFilter.h>

#include <QVector3D>

namespace glraw
{

enum class SobelMode : int
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

private:
	SobelMode m_mode;
	SobelMode ModeFromVariant(const QVariantMap& cfg);
};

}