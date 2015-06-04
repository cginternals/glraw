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

	virtual bool renderShader(std::unique_ptr<Canvas> & imageData, const QString & shader);
	virtual void setUniforms(QOpenGLShaderProgram& program);

private:

	bool createProgram(QOpenGLShaderProgram& prog, const QString & shader);
};

} // namespace glraw
