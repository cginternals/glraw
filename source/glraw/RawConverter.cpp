
#include <glraw/RawConverter.h>

#include <glraw/AssetInformation.h>

#include <QGLWidget>

namespace glraw
{

RawConverter::RawConverter()
:   m_format(RAW_GL_RGBA)
,   m_type(RAW_GL_INT)
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
    /*if (!(image.format() == QImage::Format_ARGB32 || image.format() == QImage::Format_RGB32))
        image = image.convertToFormat(QImage::Format_ARGB32);*/

    image = QGLWidget::convertToGLFormat(image);

    switch(m_type)
    {
    case RAW_GL_UNSIGNED_BYTE:
        this->write<quint8>(image, dataStream);
        break;
    case RAW_GL_BYTE:
        this->write<qint8>(image, dataStream);
        break;
    case RAW_GL_UNSIGNED_SHORT:
        this->write<quint16>(image, dataStream);
        break;
    case RAW_GL_SHORT:
        this->write<qint16>(image, dataStream);
        break;
    case RAW_GL_UNSIGNED_INT:
        this->write<quint32>(image, dataStream);
        break;
    case RAW_GL_INT:
        this->write<qint32>(image, dataStream);
        break;
    case RAW_GL_FLOAT:
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
