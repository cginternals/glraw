
#pragma once

#include <QWindow>
#include <QOpenGLFunctions_3_2_Core>

#include <QList>

class QOpenGLContext;
class QSurfaceFormat;

class Canvas : public QWindow, protected QOpenGLFunctions_3_2_Core
{
    Q_OBJECT

public:
    Canvas(
        const QSurfaceFormat & format
    ,   QScreen * screen = nullptr);
    virtual ~Canvas();

    // from QWindow
    virtual QSurfaceFormat format() const;

protected:
    const QString querys(const GLenum penum);
    const GLint queryi(const GLenum penum);

    virtual void initializeGL(const QSurfaceFormat & format);
	virtual void paintGL();

	virtual void resizeEvent(QResizeEvent * event);

    /** uses the context to verify existence/support of each mandatory extension
    */
    bool verifyExtensions() const;

protected:
    QScopedPointer<QOpenGLContext> m_context;
};
