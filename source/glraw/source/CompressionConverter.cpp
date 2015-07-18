
#include <glraw/CompressionConverter.h>

#include <glraw/AssetInformation.h>


namespace glraw
{

CompressionConverter::CompressionConverter()
:   m_compressedFormat(GL_COMPRESSED_RGBA)
{
}

QByteArray CompressionConverter::convert(std::unique_ptr<Canvas> & image, AssetInformation & info)
{   
    QByteArray imageData = image->compressedImageFromTexture(m_compressedFormat);
    
	info.setProperty("compressedFormat", static_cast<int>(m_compressedFormat));
	info.setProperty("size", imageData.size());
    
    return imageData;
}

void CompressionConverter::setCompressedFormat(GLint compressedFormat)
{
    m_compressedFormat = compressedFormat;
}

} // namespace glraw
