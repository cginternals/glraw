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

protected:
	virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass) override;
	virtual QString fragmentShaderSource(unsigned int pass) override;

	BlendMode m_blendMode;
	QVector4D m_color;
	float m_factor;

	static BlendMode GetMode(const QVariantMap& cfg);
};

}