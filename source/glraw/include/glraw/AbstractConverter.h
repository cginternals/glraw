#pragma once

#include <memory>

#include <QtGui/qopengl.h>

#include <QString>
#include <QMap>

#include <glraw/glraw_api.h>
#include <glraw/Canvas.h>

class QImage;

namespace glraw
{

class AssetInformation;

class GLRAW_API AbstractConverter
{
public:
    AbstractConverter() = default;
    virtual ~AbstractConverter() = default;

	virtual QByteArray convert(std::unique_ptr<Canvas> & image, AssetInformation & info) = 0;
};

} // namespace glraw
