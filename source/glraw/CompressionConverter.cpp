
#include <glraw/CompressionConverter.h>

#include <glraw/AssetInformation.h>

namespace glraw
{

CompressionConverter::CompressionConverter()
:   m_compressedFormat(GL_COMPRESSED_RGBA)
{
}

CompressionConverter::~CompressionConverter()
{
}

QByteArray CompressionConverter::convert(QImage & image, AssetInformation & info)
{
    info.setProperty("compressedFormat", QVariant(static_cast<int>(m_compressedFormat)));
    
    m_canvas.loadTextureFromImage(image);
    
    if (hasFragmentShader() && !m_canvas.process(m_fragmentShader))
        return QByteArray();
    
    return m_canvas.compressedImageFromTexture(m_compressedFormat);
}

void CompressionConverter::setCompressedFormat(GLint compressedFormat)
{
    m_compressedFormat = compressedFormat;
}

} // namespace glraw
