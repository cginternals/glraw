
#include <glraw/Canvas.h>

#include <cassert>

#include <QGLWidget>
#include <QtDebug>
#include <QByteArray>
#include <QImage>
#include <QOpenGLShaderProgram>
#include <QFile>

#include "UniformParser.h"

namespace
{
    
    const char * vertexShaderSource = R"(
    #version 150

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
        glDeleteTextures(1, &m_texture);
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

    if (!initializeOpenGLFunctions())
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
        glGenTextures(1, &m_texture);
    
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8
        , glImage.width(), glImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImage.bits());
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    m_context.doneCurrent();
}
    
QByteArray Canvas::imageFromTexture(GLenum format, GLenum type)
{
    assert(textureLoaded());
    
    m_context.makeCurrent(this);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    
    GLint width, height;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    
    QByteArray imageData;
    imageData.resize(numberOfElementsFor(format) * byteSizeOf(type) * width * height);
    
    glGetTexImage(GL_TEXTURE_2D, 0, format, type, imageData.data());
    
    glBindTexture(GL_TEXTURE_2D, 0);
    m_context.doneCurrent();
    
    return imageData;
}
    
QByteArray Canvas::compressedImageFromTexture(GLenum compressedInternalFormat)
{
    QByteArray uncompressedImageData = imageFromTexture(GL_RGBA, GL_UNSIGNED_BYTE);
    
    m_context.makeCurrent(this);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    
    GLint width, height;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    
    GLuint compressedTexture;
    glGenTextures(1, &compressedTexture);
    glBindTexture(GL_TEXTURE_2D, compressedTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, compressedInternalFormat, width, height, 0
        , GL_RGBA, GL_UNSIGNED_BYTE, uncompressedImageData);
    
    GLint size;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &size);
    
    QByteArray compressedImageData;
    compressedImageData.resize(size);
    glGetCompressedTexImage(GL_TEXTURE_2D, 0, compressedImageData.data());
    
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

    UniformParser::setUniforms(*this, program, uniforms);

    program.bind();


    GLint width, height;
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    
    GLuint processedTexture;
    glGenTextures(1, &processedTexture);
    glBindTexture(GL_TEXTURE_2D, processedTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, processedTexture, 0);
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    static const float rawv[] = { +1.f, -1.f, +1.f, +1.f, -1.f, -1.f, -1.f, +1.f };

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, rawv, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QVector2D), nullptr);
    glEnableVertexAttribArray(0);
    
    
    glViewport(0, 0, width, height);
    glDisable(GL_DEPTH_TEST);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    program.setUniformValue("src", 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glDeleteBuffers(1, &buffer);
    glDeleteVertexArrays(1, &vao);
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &m_texture);
    
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
