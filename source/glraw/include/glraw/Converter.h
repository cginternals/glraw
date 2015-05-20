#pragma once

#include <glraw/glraw_api.h>

#include <QtGui/qopengl.h>
#include <QString>

#include <glraw/Canvas.h>
#include <glraw/AbstractConverter.h>

class QImage;


namespace glraw
{
    
class AssetInformation;

class GLRAW_API Converter : public AbstractConverter
{
public:
    Converter();
    virtual ~Converter();

    virtual QByteArray convert(Canvas & image, AssetInformation & info);

    void setFormat(GLenum format);
    void setType(GLenum type);

protected:
    GLenum m_format;
    GLenum m_type;
};

} // namespace glraw
