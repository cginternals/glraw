
#pragma once

#include <glraw/glraw.h>

#include <cassert>
#include <QtGui/qopengl.h>
#include <QString>
#include <glraw/Canvas.h>

class QImage;

namespace glraw
{
    
class AssetInformation;

class GLRAW_API RawConverter
{
public:
    RawConverter();
    ~RawConverter();

    QByteArray convert(QImage & image, AssetInformation & info);

    void setFormat(GLenum format);
    void setType(GLenum type);
    bool setFragmentShader(const QString & sourcePath);

protected:
    GLenum m_format;
    GLenum m_type;
    QString m_fragmentShader;
    
    Canvas m_canvas;

};

} // namespace glraw
