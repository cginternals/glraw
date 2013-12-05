
#pragma once

#include <glraw/glraw.h>

#include <cassert>
#include <QtGui/qopengl.h>
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

protected:
    GLenum m_format;
    GLenum m_type;
    
    Canvas m_canvas;

};

} // namespace glraw
