
#include <glraw/Canvas.h>

#include <cassert>

#include <QGLWidget>
#include <QtDebug>
#include <QByteArray>
#include <QImage>
#include <QOpenGLShaderProgram>
#include <QFile>
#include <QOpenGLFunctions_3_2_Core>

#include <glraw/AssetInformation.h>


namespace glraw
{

Canvas::Canvas()
:   QWindow((QScreen *)nullptr)
,   m_texture(0)
,   m_gl(new QOpenGLFunctions_3_2_Core)
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
        m_gl->glDeleteTextures(1, &m_texture);
        m_context.doneCurrent();
    }
    delete m_gl;
}

void Canvas::initializeGL()
{
    QSurfaceFormat format;
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);

    m_context.setFormat(format);
    m_context.create();

    m_context.makeCurrent(this);

    if (!m_gl->initializeOpenGLFunctions())
    {
        qCritical() << "Initializing OpenGL failed.";
        return;
    }

    m_context.doneCurrent();
}
    
void Canvas::makeContext()
{
	m_context.makeCurrent(this);
}

void Canvas::doneContext()
{
	m_context.doneCurrent();
}

void Canvas::loadTexture(const QByteArray & image, AssetInformation & info)
{
	m_context.makeCurrent(this);

	if (!textureLoaded())
		m_gl->glGenTextures(1, &m_texture);

	const GLsizei w = info.property("width").toInt();
	const GLsizei h = info.property("height").toInt();

	m_gl->glBindTexture(GL_TEXTURE_2D, m_texture);
	m_gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 
		w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());

	m_gl->glBindTexture(GL_TEXTURE_2D, 0);

	m_gl->glDisable(GL_DEPTH_TEST);

	m_context.doneCurrent();
}

void Canvas::loadTextureFromImage(const QImage & image)
{
    m_context.makeCurrent(this);
    
    QImage glImage = QGLWidget::convertToGLFormat(image);
    
    if (!textureLoaded())
        m_gl->glGenTextures(1, &m_texture);
    
    m_gl->glBindTexture(GL_TEXTURE_2D, m_texture);
    m_gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8
        , glImage.width(), glImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImage.bits());
    
    m_gl->glBindTexture(GL_TEXTURE_2D, 0);
    
    m_context.doneCurrent();
}
    
QByteArray Canvas::imageFromTexture(GLenum format, GLenum type)
{
    assert(textureLoaded());
    
    m_context.makeCurrent(this);
    m_gl->glBindTexture(GL_TEXTURE_2D, m_texture);
    
    GLint width, height;
    m_gl->glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    m_gl->glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    
    QByteArray imageData;
    imageData.resize(numberOfElementsFor(format) * byteSizeOf(type) * width * height);
    
    m_gl->glGetTexImage(GL_TEXTURE_2D, 0, format, type, imageData.data());
    
    m_gl->glBindTexture(GL_TEXTURE_2D, 0);
    m_context.doneCurrent();
    
    return imageData;
}
    
QByteArray Canvas::compressedImageFromTexture(GLenum compressedInternalFormat)
{
    QByteArray uncompressedImageData = imageFromTexture(GL_RGBA, GL_UNSIGNED_BYTE);
    
    m_context.makeCurrent(this);
    m_gl->glBindTexture(GL_TEXTURE_2D, m_texture);
    
    GLint width, height;
    m_gl->glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    m_gl->glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    
    GLuint compressedTexture;
    m_gl->glGenTextures(1, &compressedTexture);
    m_gl->glBindTexture(GL_TEXTURE_2D, compressedTexture);
    m_gl->glTexImage2D(GL_TEXTURE_2D, 0, compressedInternalFormat, width, height, 0
        , GL_RGBA, GL_UNSIGNED_BYTE, uncompressedImageData);
    
    GLint size;
    m_gl->glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &size);
    
    QByteArray compressedImageData;
    compressedImageData.resize(size);
    m_gl->glGetCompressedTexImage(GL_TEXTURE_2D, 0, compressedImageData.data());
    
    m_context.doneCurrent();
    
    return compressedImageData;
}
    
void Canvas::updateTexture(GLuint tex)
{
	assert( tex );

	if(tex == m_texture)
	{
		return;
	}

	makeContext();
	m_gl->glDeleteTextures( 1, &m_texture );
	doneContext();

	m_texture = tex;
}

GLuint Canvas::texture()
{
	return m_texture;
}

QOpenGLFunctions_3_2_Core * Canvas::gl()
{
	return m_gl;
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
