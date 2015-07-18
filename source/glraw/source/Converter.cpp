
#include <glraw/Converter.h>

#include <glraw/AssetInformation.h>


namespace glraw
{

Converter::Converter()
:   m_format(GL_RGBA)
,   m_type(GL_UNSIGNED_BYTE)
{
}

QByteArray Converter::convert(std::unique_ptr<Canvas> & image, AssetInformation & info)
{  
	info.setProperty("format", static_cast<int>(m_format));
	info.setProperty("type", static_cast<int>(m_type));
    
    return image->imageFromTexture(m_format, m_type);
}

void Converter::setFormat(GLenum format)
{
    m_format = format;
}
    
void Converter::setType(GLenum type)
{
    m_type = type;
}

} // namespace glraw
