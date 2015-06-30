#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

class GLRAW_API Scale : public AbstractFilter
{
public:
	Scale( int width, int height, bool fastMode );
	Scale(const QVariantMap& cfg);
	virtual ~Scale() = default;

	virtual bool process( std::unique_ptr<Canvas> & imageData, AssetInformation & info ) override;

protected:

	void setUniforms( QOpenGLShaderProgram& program ) override;

private:

	int m_width;
	int m_height;
	bool m_fastMode;
	

	static int WidthFromVariant(const QVariantMap& cfg);
	static int HeightFromVariant(const QVariantMap& cfg);
	static bool FastModeFromVariant(const QVariantMap& cfg);
};

}