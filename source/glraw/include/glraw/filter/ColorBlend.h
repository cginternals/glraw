#pragma once
#include <glraw/filter/AbstractFilter.h>

#include <QVector4D>

namespace glraw
{

enum class BlendMode : int
{
	Mix,
	Additive,
	Subtractive,
	Difference,
	Multiply,
	Divide,
	Divisor,
	Minimum,
	Maximum,

	Default = Mix
};

class GLRAW_API ColorBlend : public AbstractFilter
{
public:
	ColorBlend( BlendMode mode, const QVector4D& color, float factor );
	ColorBlend( const QVariantMap& cfg );
	virtual ~ColorBlend() = default;

	virtual bool process( std::unique_ptr<Canvas> & imageData, AssetInformation & info ) override;

protected:

	void setUniforms( QOpenGLShaderProgram& program ) override;

private:

	BlendMode m_blendMode;
	QVector4D m_color;
	float m_factor;

	static BlendMode ModeFromVariant(const QVariantMap& cfg);
	static QVector4D ColorFromVariant( const QVariantMap& cfg );
};

}