
#include <glraw/Canvas.h>

#include <cassert>

#include <QGLWidget>
#include <QtDebug>
#include <QByteArray>
#include <QImage>
#include <QOpenGLShaderProgram>
#include <QFile>

namespace
{
    
    const char * vertexShaderSource = R"(
    #version 330

    layout(location = 0) in vec2 a_vertex;
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
        gl.glDeleteTextures(1, &m_texture);
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

    if (!gl.initializeOpenGLFunctions())
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
        gl.glGenTextures(1, &m_texture);
    
    gl.glBindTexture(GL_TEXTURE_2D, m_texture);
    gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8
        , glImage.width(), glImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImage.bits());
    
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
    
QByteArray Canvas::compressedImageFromTexture(GLenum compressedInternalFormat)
{
    QByteArray uncompressedImageData = imageFromTexture(GL_RGBA, GL_UNSIGNED_BYTE);
    
    m_context.makeCurrent(this);
    gl.glBindTexture(GL_TEXTURE_2D, m_texture);
    
    GLint width, height;
    gl.glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    gl.glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    
    GLuint compressedTexture;
    gl.glGenTextures(1, &compressedTexture);
    gl.glBindTexture(GL_TEXTURE_2D, compressedTexture);
    gl.glTexImage2D(GL_TEXTURE_2D, 0, compressedInternalFormat, width, height, 0
        , GL_RGBA, GL_UNSIGNED_BYTE, uncompressedImageData);
    
    GLint size;
    gl.glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &size);
    
    QByteArray compressedImageData;
    compressedImageData.resize(size);
    gl.glGetCompressedTexImage(GL_TEXTURE_2D, 0, compressedImageData.data());
    
    m_context.doneCurrent();
    
    return compressedImageData;
}
    
bool Canvas::process(const QString & fragmentShader)
{
    assert(textureLoaded());
    
    m_context.makeCurrent(this);
    
    QOpenGLShaderProgram program;
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
    
    program.bind();
    
    GLint width, height;
    gl.glBindTexture(GL_TEXTURE_2D, m_texture);
    gl.glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    gl.glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    
    GLuint processedTexture;
    gl.glGenTextures(1, &processedTexture);
    gl.glBindTexture(GL_TEXTURE_2D, processedTexture);
    gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    
    GLuint fbo;
    gl.glGenFramebuffers(1, &fbo);
    gl.glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    gl.glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, processedTexture, 0);
    
    GLuint vao;
    gl.glGenVertexArrays(1, &vao);
    gl.glBindVertexArray(vao);
    
    const QVector<QVector2D> quad = {
        QVector2D(-1, -1),
        QVector2D(1, -1),
        QVector2D(1, 1),
        QVector2D(-1, 1)
    };
    
    GLuint buffer;
    gl.glGenBuffers(1, &buffer);
    gl.glBindBuffer(GL_ARRAY_BUFFER, buffer);
    gl.glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D) * quad.size(), quad.data(), GL_STATIC_DRAW);
    gl.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QVector2D), nullptr);
    gl.glEnableVertexAttribArray(0);
    
    
    gl.glViewport(0, 0, width, height);
    gl.glDisable(GL_DEPTH_TEST);
    
    gl.glActiveTexture(GL_TEXTURE0);
    gl.glBindTexture(GL_TEXTURE_2D, m_texture);
    
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    program.setUniformValue("src", 0);
    
    gl.glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    gl.glDeleteBuffers(1, &buffer);
    gl.glDeleteVertexArrays(1, &vao);
    gl.glDeleteFramebuffers(1, &fbo);
    gl.glDeleteTextures(1, &m_texture);
    
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
