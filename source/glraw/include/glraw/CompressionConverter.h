#pragma once

#include <QtGui/qopengl.h>
#include <QString>

#include <glraw/glraw_api.h>

#include <glraw/Canvas.h>
#include <glraw/AbstractConverter.h>

class QImage;


namespace glraw
{
    
class AssetInformation;

class GLRAW_API CompressionConverter : public AbstractConverter
{
public:
    CompressionConverter();
    virtual ~CompressionConverter() = default;

	virtual QByteArray convert(std::unique_ptr<Canvas> & image, AssetInformation & info) override;

    void setCompressedFormat(GLint compressedFormat);

protected:
    GLint m_compressedFormat;
};

} // namespace glraw
