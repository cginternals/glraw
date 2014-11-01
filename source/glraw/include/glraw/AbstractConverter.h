#pragma once

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
    AbstractConverter();
    virtual ~AbstractConverter();

    virtual QByteArray convert(QImage & image, AssetInformation & info) = 0;

    bool hasFragmentShader() const;
    bool setFragmentShader(const QString & sourcePath);

    bool setUniform(const QString & assignment);

protected:
    Canvas m_canvas;
    QString m_fragmentShader;

    QMap<QString, QString> m_uniforms;
};

} // namespace glraw
