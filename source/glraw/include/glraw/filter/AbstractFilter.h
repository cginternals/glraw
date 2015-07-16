#pragma once

#include <glraw/glraw_api.h>

#include <memory>

#include <QVariantMap>

class QOpenGLShaderProgram;

namespace glraw
{

class AssetInformation;
class Canvas;

class GLRAW_API AbstractFilter
{
public:
    AbstractFilter() = default;
    virtual ~AbstractFilter() = default;

	virtual bool process(std::unique_ptr<Canvas> & imageData, AssetInformation & info) = 0;

protected:

	virtual void setUniforms( QOpenGLShaderProgram& program );
	virtual unsigned int numberOfPasses();

	bool renderShader(std::unique_ptr<Canvas> & imageData, const QString & shader);
	int renderShaderToTexture(std::unique_ptr<Canvas> & imageData, const QString & shader);
	bool createProgram(QOpenGLShaderProgram& prog, const QString & shader);


	static float FactorFromVariant(const QVariantMap& cfg, float default_value);
	static unsigned int SizeFromVariant(const QVariantMap& cfg, unsigned int default_value);
	static unsigned int VerifySize(unsigned int size);
};

} // namespace glraw
