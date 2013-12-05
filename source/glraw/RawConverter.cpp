
#include <glraw/RawConverter.h>

#include <QDebug>
#include <QFile>
#include <QTextStream>

#include <glraw/AssetInformation.h>

namespace glraw
{

RawConverter::RawConverter()
:   m_format(GL_RGBA)
,   m_type(GL_UNSIGNED_BYTE)
,   m_fragmentShader("")
{
}

RawConverter::~RawConverter()
{
}

QByteArray RawConverter::convert(QImage & image, AssetInformation & info)
{
    info.setProperty("format", QVariant(static_cast<int>(m_format)));
    info.setProperty("type", QVariant(static_cast<int>(m_type)));
    
    m_canvas.loadTextureFromImage(image);
    
    if (!m_fragmentShader.isEmpty() && !m_canvas.process(m_fragmentShader))
        return QByteArray();
    
    return m_canvas.imageFromTexture(m_format, m_type);
}

void RawConverter::setFormat(GLenum format)
{
    m_format = format;
}
    
void RawConverter::setType(GLenum type)
{
    m_type = type;
}
    
bool RawConverter::setFragmentShader(const QString & sourcePath)
{
    QFile file(sourcePath);
    
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Opening file" << sourcePath << "failed.";
        return false;
    }
    
    QTextStream stream(&file);
    
    m_fragmentShader = stream.readAll();
    
    return true;
}

} // namespace glraw
