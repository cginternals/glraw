
#pragma once

#include <glraw/glraw.h>

#include <QtGui/qopengl.h>
#include <QString>
#include <glraw/Canvas.h>

class QImage;

namespace glraw
{

class AssetInformation;

class GLRAW_API AbstractConverter
{
public:
    AbstractConverter();
    virtual ~AbstractConverter();

    virtual QByteArray convert(QImage & image, AssetInformation & info) = 0;

    bool hasFragmentShader() const;
    bool setFragmentShader(const QString & sourcePath);

protected:
    Canvas m_canvas;
    QString m_fragmentShader;
};

} // namespace glraw
