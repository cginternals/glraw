#pragma once
#include <glraw/filter/AbstractFilter.h>


namespace glraw
{

class GLRAW_API Rotate : public AbstractFilter
{
public:
	Rotate(int rotation);
	Rotate(const QVariantMap& cfg);
	virtual ~Rotate() = default;

	virtual bool process( std::unique_ptr<Canvas> & imageData, AssetInformation & info ) override;

protected:

	void setUniforms( QOpenGLShaderProgram& program ) override;

private:

	int m_rotation;

	static float RotationFromVariant(const QVariantMap& cfg);

};

}