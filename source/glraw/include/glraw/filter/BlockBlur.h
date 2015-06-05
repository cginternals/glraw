#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

class GLRAW_API BlockBlur : public AbstractFilter
{
public:
	BlockBlur(unsigned int range, float factor);
	BlockBlur(const QVariantMap&);
	virtual ~BlockBlur() = default;

	virtual bool process(std::unique_ptr<Canvas> & imageData, AssetInformation & info) override;

protected:

	virtual void setUniforms( QOpenGLShaderProgram& program ) override;

private:

	unsigned int m_range;
	float m_factor;

	static unsigned int VerifyRange(unsigned int range);
	static unsigned int RangeFromVariant(const QVariantMap& cfg);
	static float FactorFromVariant(const QVariantMap& cfg);
};

}