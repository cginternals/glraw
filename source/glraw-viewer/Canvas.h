
#pragma once

#include <QWindow>

#include <QVector>
#include <QVector2D>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>


class QOpenGLFunctions_3_2_Core;

class QOpenGLContext;
class QSurfaceFormat;

class Canvas : public QWindow
{
    Q_OBJECT

public:
    Canvas(
        const QSurfaceFormat & format
    ,   QScreen * screen = nullptr);
    virtual ~Canvas();

    // from QWindow
    virtual QSurfaceFormat format() const;

    void loadFile(const QString & fileName);
    void toggleResolution();

protected:
    const QString getString(const GLenum penum);
    const GLint getInteger(const GLenum penum);

    virtual void initializeGL(const QSurfaceFormat & format);
	virtual void paintGL();

	virtual void resizeEvent(QResizeEvent * event);
    virtual void exposeEvent(QExposeEvent * event);
    virtual void showEvent(QShowEvent * event);

    void printHardwareInfo();
    /** uses the context to verify existence/support of each mandatory extension
    */
    bool verifyExtensions() const;

	void invalidate();

	bool loadGLRawImage(const QString & fileName);
	bool loadRawImage(const QString & fileName);
	bool loadQImage(const QString & fileName);

protected:
    QScopedPointer<QOpenGLContext> m_context;
    GLuint m_texture;

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vertices;

    QOpenGLShaderProgram * m_program;
    QSize m_textureSize;

    bool m_valid;
    bool m_actualResolution;

    // using gl as a memeber instead of inheritance 
    // probably resolves an deinitialization issue.
    QOpenGLFunctions_3_2_Core * m_gl;
};
