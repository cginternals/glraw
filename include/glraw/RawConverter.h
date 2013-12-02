
#pragma once

#include <glraw/glraw.h>

#include <cassert>

#include <QtGui/qopengl.h>
#include <QImage>
#include <QDataStream>

#include <glraw/Canvas.h>

namespace glraw
{
    
class AssetInformation;

class GLRAW_API RawConverter
{
public:
    RawConverter();
    ~RawConverter();

    void updateAssetInformation(AssetInformation & info);
    void convert(QImage & image, QDataStream & dataStream);

    void setFormat(GLenum format);
    void setType(GLenum type);

protected:
    GLenum m_format;
    GLenum m_type;
    
    Canvas m_canvas;

};

} // namespace glraw
