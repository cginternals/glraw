
#include <glraw/RawConverter.h>

#include <glraw/AssetInformation.h>

namespace glraw
{

RawConverter::RawConverter()
:   m_format(GL_RGBA)
,   m_type(GL_UNSIGNED_BYTE)
{
}

RawConverter::~RawConverter()
{
}

void RawConverter::updateAssetInformation(AssetInformation & info)
{
    info.setProperty("format", QVariant(static_cast<int>(m_format)));
    info.setProperty("type", QVariant(static_cast<int>(m_type)));
}

void RawConverter::convert(QImage & image, QDataStream & dataStream)
{
    m_canvas.loadTextureFromImage(image);
    
    QByteArray imageData = m_canvas.imageFromTexture(m_format, m_type);
    dataStream << imageData;
}

void RawConverter::setFormat(GLenum format)
{
    m_format = format;
}
void RawConverter::setType(GLenum type)
{
    m_type = type;
}

} // namespace glraw
