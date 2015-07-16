#pragma once
#include <glraw/filter/AbstractFilter.h>


namespace glraw
{

class GLRAW_API Rotate : public AbstractFilter
{
public:
	Rotate(unsigned int rotation);
	Rotate(const QVariantMap& cfg);
	virtual ~Rotate() = default;

	virtual bool process( std::unique_ptr<Canvas> & imageData, AssetInformation & info ) override;

	enum class RotationMode : int
	{
		RotateBy_0,
		RotateBy_90,
		RotateBy_180,
		RotateBy_270,

		Default = RotateBy_90
	};

protected:

	void setUniforms( QOpenGLShaderProgram& program ) override;

private:

	unsigned int m_rotation;

	static unsigned int RotationFromVariant(const QVariantMap& cfg);
};

}