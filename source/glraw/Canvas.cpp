
#include <glraw/Canvas.h>

#include <cassert>

#include <QGLWidget>
#include <QtDebug>
#include <QByteArray>
#include <QImage>
#include <QOpenGLShaderProgram>
#include <QFile>
#include <QOpenGLFunctions_3_2_Core>

#include "UniformParser.h"

namespace
{    
    const char * vertexShaderSource = 
    R"(#version 150

    in vec2 a_vertex;
    out vec2 v_uv;

    void main()
    {
        v_uv = a_vertex.xy * 0.5 + 0.5;    
        gl_Position = vec4(a_vertex * 1.0, 0.0, 1.0);
    }
    )";
}

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
    
bool Canvas::process(
    const QString & fragmentShader
,   const QMap<QString, QString> & uniforms)
{
    assert(textureLoaded());
    
    m_context.makeCurrent(this);
    
    QOpenGLShaderProgram program;
    program.bindAttributeLocation("a_vertex", 0);

    program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    
    if (!program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader))
    {
        qDebug() << program.log();
        return false;
    }
    
    if (!program.link())
    {
        qDebug() << program.log();
        return false;
    }

    UniformParser::setUniforms(*m_gl, program, uniforms);

    program.bind();


    GLint width, height;
    m_gl->glBindTexture(GL_TEXTURE_2D, m_texture);
    m_gl->glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    m_gl->glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    
    GLuint processedTexture;
    m_gl->glGenTextures(1, &processedTexture);
    m_gl->glBindTexture(GL_TEXTURE_2D, processedTexture);
    m_gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    
    GLuint fbo;
    m_gl->glGenFramebuffers(1, &fbo);
    m_gl->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    m_gl->glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, processedTexture, 0);
    
    GLuint vao;
    m_gl->glGenVertexArrays(1, &vao);
    m_gl->glBindVertexArray(vao);
    
    static const float rawv[] = { +1.f, -1.f, +1.f, +1.f, -1.f, -1.f, -1.f, +1.f };

    GLuint buffer;
    m_gl->glGenBuffers(1, &buffer);
    m_gl->glBindBuffer(GL_ARRAY_BUFFER, buffer);
    m_gl->glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 8, rawv, GL_STATIC_DRAW);
    m_gl->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QVector2D), nullptr);
    m_gl->glEnableVertexAttribArray(0);
    
    
    m_gl->glViewport(0, 0, width, height);
    m_gl->glDisable(GL_DEPTH_TEST);
    
    m_gl->glActiveTexture(GL_TEXTURE0);
    m_gl->glBindTexture(GL_TEXTURE_2D, m_texture);
    
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    program.setUniformValue("src", 0);

    m_gl->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    m_gl->glDeleteBuffers(1, &buffer);
    m_gl->glDeleteVertexArrays(1, &vao);
    m_gl->glDeleteFramebuffers(1, &fbo);
    m_gl->glDeleteTextures(1, &m_texture);
    
    program.release();
    
    m_context.doneCurrent();

    m_texture = processedTexture;
    
    return true;
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
