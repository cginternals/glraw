#pragma once
#include <glraw/filter/AbstractFilter.h>


namespace glraw
{

class GLRAW_API Mirror : public AbstractFilter
{
public:
	Mirror(bool horizontal, bool vertical);
	Mirror(const QVariantMap& cfg);
	virtual ~Mirror() = default;

	virtual bool process( std::unique_ptr<Canvas> & imageData, AssetInformation & info ) override;

protected:

	void setUniforms( QOpenGLShaderProgram& program ) override;

private:

	bool m_horizontal;
	bool m_vertical;

	static bool HorizontalFromVariant( const QVariantMap& cfg );
	static bool VerticalFromVariant(const QVariantMap& cfg);
};

}