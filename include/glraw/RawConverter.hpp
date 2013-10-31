
#pragma once

namespace glraw
{
    
template <typename Type>
void RawConverter::write(const QImage & image, QDataStream & dataStream)
{
    const bool r = (GL_RED == m_format || GL_RG == m_format || GL_RGB == m_format || GL_RGBA == m_format || GL_BGR == m_format || GL_BGRA == m_format);
    const bool g = (GL_RG == m_format || GL_RGB == m_format || GL_RGBA == m_format || GL_BGR == m_format || GL_BGRA == m_format);
    const bool b = (GL_RGB == m_format || GL_RGBA == m_format || GL_BGR == m_format || GL_BGRA == m_format);
    const bool a = (GL_RGBA == m_format || GL_BGRA == m_format);

    // const bool grayscale = (GL_DEPTH_COMPONENT == format || GL_DEPTH_STENCIL == format || GL_STENCIL_INDEX == format);

    const int w = image.width();
    const int h = image.height();
    const int s = w * h;

    const float scale = std::is_same<Type, float>::value ? 1.f / 255.f : 1.f;

    assert(image.byteCount() == s * 4);
    for(int i = 0; i < s * 4; i += 4)
    {
        if (r)
            dataStream << static_cast<Type>(scale * image.constBits()[i + 0]);
        if (g)
            dataStream << static_cast<Type>(scale * image.constBits()[i + 1]);
        if (b)
            dataStream << static_cast<Type>(scale * image.constBits()[i + 2]);
        if (a)
            dataStream << static_cast<Type>(scale * image.constBits()[i + 3]);
    }
}

} // namespace glraw
