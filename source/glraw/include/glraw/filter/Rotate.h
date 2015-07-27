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

	enum class RotationMode : int
	{
		RotateBy_0,
		RotateBy_90,
		RotateBy_180,
		RotateBy_270,

		Default = RotateBy_90
	};

protected:
	virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass) override;
	virtual QString fragmentShaderSource(unsigned int pass) override;

	virtual int createWorkingTexture(unsigned int prototype) override;
	virtual void updateAssetInformation(AssetInformation & info) override;

	unsigned int m_rotation;

	int out_width;
	int out_height;

};

}