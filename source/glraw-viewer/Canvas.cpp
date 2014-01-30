
#include "Canvas.h"

#include <cassert>

#include <QDebug>
#include <QApplication>
#include <QResizeEvent>
#include <QOpenGLFunctions_3_2_Core>

#include <glraw/RawFile.h>


namespace 
{
    const char* vertexShaderSource =
    R"(#version 150

    in vec2 a_vertex;
    out vec2 v_uv;

    void main()
    {
        v_uv = a_vertex.xy * 0.5 + 0.5;
        gl_Position = vec4(a_vertex * 0.96, 0.0, 1.0);
    }
    )";

    const char* fragmentShaderSource =
    R"(#version 150

    uniform sampler2D src;
    in vec2 v_uv;

    out vec4 dst;

    void main()
    {
        vec2 moduv = mod(ivec2(gl_FragCoord.xy * 0.125), ivec2(2));
        float pattern = mix(0.8, 1.0, (moduv.x == moduv.y));
        vec4 color = texture(src, v_uv);
        dst = mix(vec4(vec3(pattern), 1.0), color, color.a);
    }
    )";
}


Canvas::Canvas(
    const QSurfaceFormat & format
,   QScreen * screen)
: QWindow(screen)
, m_context(new QOpenGLContext)
, m_vertices(QOpenGLBuffer::VertexBuffer)
, m_texture(-1)
, m_validTexture(false)
, m_gl(new QOpenGLFunctions_3_2_Core)
{
    setSurfaceType(OpenGLSurface);    

    create();
    initializeGL(format);
}

Canvas::~Canvas()
{  
    m_gl->glDeleteTextures(1, &m_texture);
    delete m_program;

    delete m_gl;
}

QSurfaceFormat Canvas::format() const
{
    if (!m_context)
        return QSurfaceFormat();

    return m_context->format();
}

const QString Canvas::getString(const GLenum penum)
{
    const QString result = reinterpret_cast<const char*>(m_gl->glGetString(penum));
    return result;
}

const GLint Canvas::getInteger(const GLenum penum)
{
    GLint result;
    m_gl->glGetIntegerv(penum, &result);

    return result;
}

void Canvas::initializeGL(const QSurfaceFormat & format)
{
    m_context->setFormat(format);
    m_context->create();

    m_context->makeCurrent(this);
    m_gl->initializeOpenGLFunctions();


    m_vao.create();
    m_vao.bind();

    m_vertices.create();
    m_vertices.setUsagePattern(QOpenGLBuffer::StaticDraw);

    static const float rawv[] = { -1.f, +1.f, +1.f, +1.f, -1.f, -1.f, +1.f, -1.f };

    m_vertices.bind();
    m_vertices.allocate(rawv, sizeof(float) * 8);

    m_gl->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)* 0, nullptr);
    m_gl->glEnableVertexAttribArray(0);

    m_vao.release();


    m_gl->glGenTextures(1, &m_texture);
    m_gl->glBindTexture(GL_TEXTURE_2D, m_texture);

    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    m_gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_FLOAT, 0);

    m_gl->glBindTexture(GL_TEXTURE_2D, 0);



    m_program = new QOpenGLShaderProgram(this);
    m_program->bindAttributeLocation("a_vertex", 0);

    auto vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    auto fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);

    vertexShader->compileSourceCode(vertexShaderSource);
    fragmentShader->compileSourceCode(fragmentShaderSource);
    m_program->addShader(vertexShader);
    m_program->addShader(fragmentShader);
    m_program->link();

    m_program->bind();
    m_program->setUniformValue("src", 0);

    printHardwareInfo();
    verifyExtensions(); // false if no painter ...

    m_gl->glClearColor(0.16f, 0.16f, 0.16f, 1.f);

    m_context->doneCurrent();
}

void Canvas::showEvent(QShowEvent *)
{
    resize(size()); // fixes initial viewport size
}

void Canvas::resizeEvent(QResizeEvent *)
{
    /* Setting the viewport here is useless, because QOpenGLContext::makeCurrent
       calls internally glViewport() on OS X. */
}

void Canvas::exposeEvent(QExposeEvent *)
{
    if (isExposed())
        paintGL();
}

void Canvas::paintGL()
{
    m_context->makeCurrent(this);

    m_gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_validTexture && !m_textureSize.isNull())
    {
        QSize size = m_textureSize.scaled(width(), height(), Qt::KeepAspectRatio);
        QPoint position((width() - size.width()) * 0.5f, (height() - size.height()) * 0.5f);

        size *= devicePixelRatio();
        position *= devicePixelRatio();

        m_gl->glViewport(position.x(), position.y(), size.width(), size.height());

        m_gl->glEnable(GL_BLEND);
        m_gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_gl->glActiveTexture(GL_TEXTURE0);
        m_gl->glBindTexture(GL_TEXTURE_2D, m_texture);

        m_program->bind();

        m_vao.bind();
        m_gl->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        m_vao.release();

        m_program->release();

        m_gl->glBindTexture(GL_TEXTURE_2D, 0);
        m_gl->glDisable(GL_BLEND);
    }

    m_context->swapBuffers(this);
    m_context->doneCurrent();
}

void Canvas::printHardwareInfo()
{
    qDebug();
    qDebug().nospace() << "GPU: "
        << qPrintable(getString(GL_RENDERER)) << " ("
        << qPrintable(getString(GL_VENDOR)) << ", "
        << qPrintable(getString(GL_VERSION)) << ")";
    qDebug().nospace() << "GL Version: "
        << qPrintable(QString::number(getInteger(GL_MAJOR_VERSION))) << "."
        << qPrintable(QString::number(getInteger(GL_MINOR_VERSION))) << " "
        << (getInteger(GL_CONTEXT_CORE_PROFILE_BIT) ? "Core" : "Compatibility");
    qDebug();
}

bool Canvas::verifyExtensions() const
{
    if (!m_context->isValid())
    {
        qWarning("Extensions cannot be veryfied due to invalid context.");
        return false;
    }

    QStringList unsupported;

    const QStringList extensions; // ToDo: add mandatory extensions
    foreach(const QString & extension, extensions)
    if (!m_context->hasExtension(qPrintable(extension)))
        unsupported << extension;

    if (unsupported.isEmpty())
        return true;

    if (unsupported.size() > 1)
        qWarning("The following mandatory OpenGL extensions are not supported:");
    else
        qWarning("The following mandatory OpenGL extension is not supported:");

    foreach(const QString & extension, unsupported)
        qWarning() << qPrintable(extension);

    qWarning() << "";

    return false;
}

void Canvas::loadFile(const QString & fileName)
{
    /*// filename.2560.1920.rgba.i.glraw
    QRegExp regExp(R"(^.*\.(\d+)\.(\d+)\.(\w+)\.(\w+)\.glraw$)");

    bool match = regExp.exactMatch(filename);

    if (!match)
        return;

    QStringList parts = regExp.capturedTexts();

    int w = parts[1].toInt();
    int h = parts[2].toInt();
    QString formatString = parts[3].toLower();
    QString typeString = parts[4].toLower();*/
    
    m_gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glraw::RawFile rawFile(fileName.toStdString());
    if (!rawFile.isValid())
    {
        qWarning() << "Reading raw file " << fileName << " failed.";

        m_validTexture = false;
        paintGL();
        return;
    }

    int w = rawFile.intProperty("width");
    int h = rawFile.intProperty("height");

    m_context->makeCurrent(this);
    
    m_gl->glBindTexture(GL_TEXTURE_2D, m_texture);

    if (rawFile.hasIntProperty("format"))
    {
        GLenum format = static_cast<GLenum>(rawFile.intProperty("format"));
        GLenum type = static_cast<GLenum>(rawFile.intProperty("type"));

        m_gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, format, type, rawFile.data());
    }
    else
    {
        GLenum compressedFormat = static_cast<GLenum>(rawFile.intProperty("compressedFormat"));
        GLenum size = rawFile.intProperty("size");

        m_gl->glCompressedTexImage2D(GL_TEXTURE_2D, 0, compressedFormat, w, h, 0, size, rawFile.data());
    }
    m_gl->glBindTexture(GL_TEXTURE_2D, 0);

    m_textureSize = QSize(w, h); 

    m_context->doneCurrent();

    qWarning() << "Reading raw file " << fileName << " succeeded.";

    m_validTexture = true;
    paintGL();
}
