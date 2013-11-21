
#include <glraw/RawConverter.h>

#include <glraw/AssetInformation.h>

namespace glraw
{

RawConverter::RawConverter()
:   m_format(GL_RGBA)
,   m_type(GL_INT)
{
}

RawConverter::~RawConverter()
{
}

void RawConverter::updateAssetInformation(AssetInformation & info)
{
    info.setProperty("format", QVariant(m_format));
    info.setProperty("type", QVariant(m_type));
}

void RawConverter::convert(QImage & image, QDataStream & dataStream)
{
    if (!(image.format() == QImage::Format_ARGB32 || image.format() == QImage::Format_RGB32))
        image = image.convertToFormat(QImage::Format_ARGB32);

    switch(m_type)
    {
    case GL_UNSIGNED_BYTE:
        this->write<quint8>(image, dataStream);
        break;
    case GL_BYTE:
        this->write<qint8>(image, dataStream);
        break;
    case GL_UNSIGNED_SHORT:
        this->write<quint16>(image, dataStream);
        break;
    case GL_SHORT:
        this->write<qint16>(image, dataStream);
        break;
    case GL_UNSIGNED_INT:
        this->write<quint32>(image, dataStream);
        break;
    case GL_INT:
        this->write<qint32>(image, dataStream);
        break;
    case GL_FLOAT:
        this->write<float>(image, dataStream);
        break;
    }
}

void RawConverter::setFormat(Format format)
{
    m_format = format;
}
void RawConverter::setType(Type type)
{
    m_type = type;
}

} // namespace glraw
