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

	virtual bool process( std::unique_ptr<Canvas> & imageData, AssetInformation & info ) override;

protected:

	void setUniforms( QOpenGLShaderProgram& program ) override;

private:

	bool m_invertAlpha;

	static bool AlphaFromVariant( const QVariantMap& cfg );
};

}