#pragma once
#include <glraw/filter/AbstractFilter.h>


namespace glraw
{

class GLRAW_API Invert : public AbstractFilter
{
public:
	Invert( bool invert_alpha );
	Invert( const QVariantMap& cfg );
	virtual ~Invert() = default;

protected:

	virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass) override;
	virtual QString fragmentShaderSource(unsigned int pass) override;

private:

	bool m_invertAlpha;

	static bool AlphaFromVariant( const QVariantMap& cfg );
};

}