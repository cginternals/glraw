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

	virtual bool process(Canvas & imageData, AssetInformation & info);

private:

	QVector3D m_conversionFactor;

	static QVector3D FactorFromMode(GrayscaleFactor in);
	static QVector3D FactorFromVariant(const QVariantMap& cfg);
};

}