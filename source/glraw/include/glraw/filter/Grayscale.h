#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

enum class GrayscaleFactor
{
	Classic,
	Modern,

	Default = Modern
};

class GLRAW_API Grayscale : public AbstractFilter
{
public:
	Grayscale(GrayscaleFactor in);
	Grayscale(const QVector3D& factor);
	Grayscale(const QVariantMap& cfg);
	virtual ~Grayscale() = default;

protected:
	virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass) override;
	virtual QString fragmentShaderSource(unsigned int pass) override;

	QVector3D m_conversionFactor;

	static QVector3D GetFactor(GrayscaleFactor in);
	static QVector3D GetFactor(const QVariantMap& cfg);
};

}