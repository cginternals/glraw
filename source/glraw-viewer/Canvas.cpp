
#include <cassert>

#include <QDebug>
#include <QApplication>
#include <QResizeEvent>

#include "Canvas.h"


Canvas::Canvas(
    const QSurfaceFormat & format
,   QScreen * screen)
: QWindow(screen)
, m_context(new QOpenGLContext)
{
    setSurfaceType(OpenGLSurface);    

    create();
    initializeGL(format);
}

Canvas::~Canvas()
{
}

QSurfaceFormat Canvas::format() const
{
    if (!m_context)
        return QSurfaceFormat();

    return m_context->format();
}

const QString Canvas::getString(const GLenum penum)
{
    const QString result = reinterpret_cast<const char*>(glGetString(penum));
    return result;
}

const GLint Canvas::getInteger(const GLenum penum)
{
    GLint result;
    glGetIntegerv(penum, &result);

    return result;
}

namespace {

const char* vertexShaderSource =
R"(#version 330

layout(location = 0) in vec2 in_vertex;
out vec2 texCoord;

void main()
{
    gl_Position = vec4(in_vertex, 0.0, 1.0);
    texCoord = (in_vertex+vec2(1.0, 1.0))/2.0;
}

)";

const char* fragmentShaderSource =
R"(#version 330

uniform sampler2D u_texture;
in vec2 texCoord;

layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = texture(u_texture, texCoord);
    //fragColor = vec4(texCoord, 0.0, 1.0);
}

)";

}

QVector<QVector2D> Canvas::m_quad = {
    QVector2D(-1, -1),
    QVector2D(1, -1),
    QVector2D(1, 1),
    QVector2D(-1, 1)
};

void Canvas::initializeGL(const QSurfaceFormat & format)
{
    m_context->setFormat(format);
    m_context->create();

    m_context->makeCurrent(this);
    initializeOpenGLFunctions();

    glGenTextures(1, &m_textureHandle);
    glBindTexture(GL_TEXTURE_2D, m_textureHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int w = 10;
    int h = 10;
    int* data = new int[w*h*4];
    for (int i=0; i<w*h*4; ++i)
    {
        data[i] = i/(w*h*4.0)*std::numeric_limits<int>::max();
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_INT, data);
    delete data;

    glBindTexture(GL_TEXTURE_2D, 0);

    glGenVertexArrays(1, &m_vaoHandle);
    glBindVertexArray(m_vaoHandle);

    glGenBuffers(1, &m_vboHandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D)*4, m_quad.constData(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QVector2D), nullptr);
    glEnableVertexAttribArray(0);

    m_program = new QGLShaderProgram(this);
    auto vertexShader = new QGLShader(QGLShader::Vertex);
    auto fragmentShader = new QGLShader(QGLShader::Fragment);
    vertexShader->compileSourceCode(vertexShaderSource);
    fragmentShader->compileSourceCode(fragmentShaderSource);
    m_program->addShader(vertexShader);
    m_program->addShader(fragmentShader);
    m_program->link();

    m_program->bind();
    m_program->setUniformValue("texture", 0);

    printHardwareInfo();
    verifyExtensions(); // false if no painter ...

    m_context->doneCurrent();
}

void Canvas::resizeEvent(QResizeEvent * event)
{
    m_context->makeCurrent(this);

    int side = qMin(width(), height());
    glViewport((width() - side) / 2, (height() - side) / 2, side, side);

    m_context->doneCurrent();
}

void Canvas::exposeEvent(QExposeEvent * event)
{
    if (isExposed())
    {
        paintGL();
    }
}

void Canvas::paintGL()
{
    m_context->makeCurrent(this);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureHandle);

    m_program->bind();

    glBindVertexArray(m_vaoHandle);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);

    m_program->release();

    glBindTexture(GL_TEXTURE_2D, 0);

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

#include <QMessageBox>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
//#include <glraw/RawFile.h>

void Canvas::loadFile(const QString & filename)
{
    // filename.2560.1920.rgba.i.glraw
    QRegExp regExp(R"(^.*\.(\d+)\.(\d+)\.(\w+)\.(\w+)\.glraw$)");

    bool match = regExp.exactMatch(filename);

    if (!match)
        return;

    QStringList parts = regExp.capturedTexts();

    int w = parts[1].toInt();
    int h = parts[2].toInt();
    QString formatString = parts[3].toLower();
    QString typeString = parts[4].toLower();

    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(nullptr, "Error", QString("Could not open file %1: %2").arg(filename).arg(file.errorString()));
        return;
    }

    QByteArray bytes = file.readAll();

    static QByteArray magicNumber("c6f5");

    quint64 offset = 0;
    QByteArray data;

    if (bytes.startsWith(magicNumber))
    {
        offset = *reinterpret_cast<quint64*>(bytes.mid(magicNumber.size()).data());
        data = bytes.mid(offset);

        assert(bytes.mid(offset).size() >= w*h*4*4);
    }
    else
    {
        file.close();
        return;
    }

    file.close();

    for (int i = 0; i<w*h*4; ++i)
    {
        int wrong = ((int*)data.data())[i];
        int right = wrong/255.0 * std::numeric_limits<int>::max();
        ((int*)data.data())[i] = right;
    }

    m_context->makeCurrent(this);

    glBindTexture(GL_TEXTURE_2D, m_textureHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_INT, data.data());
    glBindTexture(GL_TEXTURE_2D, 0);


    paintGL();
}
