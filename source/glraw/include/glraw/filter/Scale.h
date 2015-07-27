#pragma once
#include <glraw/filter/AbstractFilter.h>

#include <QVector2D>

namespace glraw
{

enum class ScaleMode : int
{
	Absolute,
	Relative,
	RatioX,
	RatioY,

	Default = Relative
};

class GLRAW_API Scale : public AbstractFilter
{
public:
	Scale(float scale, bool bilinear);
	Scale(unsigned int width, unsigned int height, bool bilinear);
	Scale(unsigned int side, bool x_axis, bool bilinear);
	Scale(const QVariantMap& cfg);
	virtual ~Scale() = default;

protected:
	virtual void updateAssetInformation(AssetInformation & info) override;
	virtual QString fragmentShaderSource(unsigned int pass) override;
	virtual int createWorkingTexture(unsigned int prototype) override;
	virtual void bindTexture(unsigned int unit, unsigned int tex) override;

	ScaleMode m_mode;
	QVector2D m_size;
	bool m_bilinear;

	std::pair<int,int> m_output;

	QVector2D calculateSize(int width, int height) const;

	static ScaleMode ModeFromVariant(const QVariantMap& cfg);
	static QVector2D GetSize(ScaleMode mode, const QVariantMap& cfg);
};

}