
#include <glraw/Canvas.h>

#include <cassert>
#include <QtDebug>

namespace glraw
{

Canvas::Canvas()
:   QWindow((QScreen *)nullptr)
,   m_texture(0)
{
    setSurfaceType(OpenGLSurface);
    create();
    
    initializeGL();
}

Canvas::~Canvas()
{
    if (textureLoaded())
    {
        m_context.makeCurrent(this);
        gl.glDeleteTextures(1, &m_texture);
        m_context.doneCurrent();
    }
}

void Canvas::initializeGL()
{
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);

    m_context.setFormat(format);
    m_context.create();

    m_context.makeCurrent(this);

    if (!gl.initializeOpenGLFunctions())
    {
        qCritical() << "Initializing OpenGL failed.";
        return;
    }

    m_context.doneCurrent();
}
    
void Canvas::loadTextureFromImage(QImage & image)
{
    m_context.makeCurrent(this);
    
    if (image.format() == QImage::Format_RGB32 || image.format() == QImage::Format_ARGB32)
        image = image.convertToFormat(QImage::Format_ARGB32);
    
    image = image.mirrored();
    
    if (!textureLoaded())
        gl.glGenTextures(1, &m_texture);
    
    gl.glBindTexture(GL_TEXTURE_2D, m_texture);
    gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 image.width(), image.height(), 0,
                 GL_BGRA, GL_UNSIGNED_BYTE,
                 image.bits());
    
    gl.glBindTexture(GL_TEXTURE_2D, 0);
    
    m_context.doneCurrent();
}
    
QByteArray Canvas::imageFromTexture(GLenum format, GLenum type)
{
    assert(textureLoaded());
    
    m_context.makeCurrent(this);
    gl.glBindTexture(GL_TEXTURE_2D, m_texture);
    
    GLint width, height;
    gl.glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    gl.glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    
    QByteArray imageData;
    imageData.resize(numberOfElementsFor(format) * byteSizeOf(type) * width * height);
    gl.glGetTexImage(GL_TEXTURE_2D, 0, format, type, imageData.data());
    
    gl.glBindTexture(GL_TEXTURE_2D, 0);
    m_context.doneCurrent();
    
    return imageData;
}
    
bool Canvas::textureLoaded() const
{
    return m_texture != 0;
}
    
int Canvas::byteSizeOf(GLenum type)
{
    switch (type)
    {
        case GL_UNSIGNED_BYTE:
        case GL_BYTE:
            return sizeof(GLbyte);
            break;
        case GL_UNSIGNED_SHORT:
        case GL_SHORT:
            return sizeof(GLshort);
        case GL_UNSIGNED_INT:
        case GL_INT:
            return sizeof(GLint);
        case GL_FLOAT:
            return sizeof(GLfloat);
        default:
            qFatal("Unsupported type passed.");
            return -1;
    }
}
    
int Canvas::numberOfElementsFor(GLenum format)
{
    switch (format)
    {
        case GL_RED:
        case GL_GREEN:
        case GL_BLUE:
            return 1;
        case GL_RG:
            return 2;
        case GL_RGB:
        case GL_BGR:
            return 3;
        case GL_RGBA:
        case GL_BGRA:
            return 4;
        default:
            qFatal("Unsupported format passed.");
            return -1;
    }
}
    
} // namespace glraw
