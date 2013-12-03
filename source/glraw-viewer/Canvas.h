
#pragma once

#include <QWindow>
#include <QOpenGLFunctions_3_3_Core>

#include <QVector>
#include <QVector2D>
#include <QGLShaderProgram>

class QOpenGLContext;
class QSurfaceFormat;

class Canvas : public QWindow, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    Canvas(
        const QSurfaceFormat & format
    ,   QScreen * screen = nullptr);
    virtual ~Canvas();

    // from QWindow
    virtual QSurfaceFormat format() const;

    void loadFile(const QString & filename);

protected:
    const QString getString(const GLenum penum);
    const GLint getInteger(const GLenum penum);

    virtual void initializeGL(const QSurfaceFormat & format);
	virtual void paintGL();

	virtual void resizeEvent(QResizeEvent * event);
    virtual void exposeEvent(QExposeEvent * event);

    void printHardwareInfo();
    /** uses the context to verify existence/support of each mandatory extension
    */
    bool verifyExtensions() const;

protected:
    QScopedPointer<QOpenGLContext> m_context;

    GLuint m_textureHandle;
    GLuint m_vaoHandle;
    GLuint m_vboHandle;
    static QVector<QVector2D> m_quad;
    QGLShaderProgram* m_program;
};
