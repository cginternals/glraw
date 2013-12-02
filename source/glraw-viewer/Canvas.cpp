
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

const QString Canvas::querys(const GLenum penum) 
{
    const QString result = reinterpret_cast<const char*>(glGetString(penum));
    return result;
}

const GLint Canvas::queryi(const GLenum penum)
{
    GLint result;
    glGetIntegerv(penum, &result);

    return result;
}

void Canvas::initializeGL(const QSurfaceFormat & format)
{
    m_context->setFormat(format);
    m_context->create();

    m_context->makeCurrent(this);
    initializeOpenGLFunctions();

    // print some hardware information

    qDebug();
    qDebug().nospace() << "GPU: " 
        << qPrintable(querys(GL_RENDERER)) << " ("
        << qPrintable(querys(GL_VENDOR)) << ", "
        << qPrintable(querys(GL_VERSION)) << ")";
    qDebug().nospace() << "GL Version: "
        << qPrintable(QString::number(queryi(GL_MAJOR_VERSION))) << "."
        << qPrintable(QString::number(queryi(GL_MINOR_VERSION))) << " "
        << (queryi(GL_CONTEXT_CORE_PROFILE_BIT) ? "Core" : "Compatibility");
    qDebug();

    verifyExtensions(); // false if no painter ...
}

void Canvas::resizeEvent(QResizeEvent * event)
{
    // ToDo: set viewport and ortho, then update shader uniforms (transform)



    if (isExposed() && Hidden != visibility())
        paintGL();
}

void Canvas::paintGL()
{
    if (!isExposed() || Hidden == visibility())
        return;

    m_context->makeCurrent(this);

    // ToDo: paint stuff


    m_context->swapBuffers(this);
    m_context->doneCurrent();
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
