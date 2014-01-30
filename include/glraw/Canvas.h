
#pragma once

#include <QWindow>
#include <QOpenGLFunctions_3_2_Core>

#include <glraw/glraw.h>

class QImage;
class QByteArray;

class QOpenGLFunctions_3_2_Core;

namespace glraw
{

class GLRAW_API Canvas : public QWindow
{
public:
    Canvas();
    virtual ~Canvas();

    void initializeGL();

    void loadTextureFromImage(const QImage & image);
    QByteArray imageFromTexture(GLenum format, GLenum type);
    QByteArray compressedImageFromTexture(GLenum compressedInternalFormat);

    bool process(
        const QString & fragmentShader
    ,   const QMap<QString, QString> & uniforms);

    bool textureLoaded() const;

protected:
    static int byteSizeOf(GLenum type);
    static int numberOfElementsFor(GLenum format);
    
    QOpenGLContext m_context;
    GLuint m_texture;   

    // using gl as a memeber instead of inheritance 
    // probably resolves an deinitialization issue on os x
    QOpenGLFunctions_3_2_Core * m_gl;
};

} // namespace glraw
