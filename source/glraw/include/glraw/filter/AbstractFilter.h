#pragma once

#include <glraw/glraw_api.h>

#include <memory>

#include <QVariantMap>

class QOpenGLShaderProgram;
class QOpenGLFunctions_3_2_Core;

namespace glraw
{

class AssetInformation;
class Canvas;

class GLRAW_API AbstractFilter
{
public:
    AbstractFilter();
    virtual ~AbstractFilter() = default;

	bool process(std::unique_ptr<Canvas> & imageData, AssetInformation & info);

protected:

	virtual unsigned int numberOfPasses();
	virtual QString fragmentShaderSource(unsigned int pass) = 0;
	virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass);
	virtual void updateAssetInformation(AssetInformation & info);
	virtual int createWorkingTexture(unsigned int prototype);
	
	int renderToTexture(std::unique_ptr<Canvas> & imageData);
	bool createProgram(QOpenGLShaderProgram& prog, const QString & shader);
	void bindTexture(unsigned int unit, unsigned int tex);

	QOpenGLFunctions_3_2_Core * m_gl;

	static float FactorFromVariant(const QVariantMap& cfg, float default_value);
	static unsigned int SizeFromVariant(const QVariantMap& cfg, unsigned int default_value);
	static unsigned int VerifySize(unsigned int size);

	void attachToFramebuffer(unsigned int texture);
	void bindVertexBuffer();

	enum Pass : int
	{
		First,
		Second,
	};
};

} // namespace glraw
