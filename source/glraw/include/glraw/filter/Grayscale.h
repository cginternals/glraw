#pragma once
#include <glraw/filter/AbstractFilter.h>

#include <QVector3D>

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
	Grayscale(GrayscaleFactor in = GrayscaleFactor::Default);
	Grayscale(const QVector3D& factor);
	Grayscale(const QVariantMap& cfg);
	virtual ~Grayscale() = default;

	virtual bool process(std::unique_ptr<Canvas> & imageData, AssetInformation & info) override;

protected:

	void setUniforms(QOpenGLShaderProgram& program) override;

private:

	QVector3D m_conversionFactor;

	static QVector3D FactorFromMode(GrayscaleFactor in);
	static QVector3D FactorFromVariant(const QVariantMap& cfg);
};

}