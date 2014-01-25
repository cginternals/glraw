
#pragma once


#include <QWindow>
#include <QOpenGLFunctions_3_2_Core>

#include <glraw/glraw.h>

class QImage;
class QByteArray;

namespace glraw
{

class GLRAW_API Canvas : public QWindow, protected QOpenGLFunctions_3_2_Core
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
};

} // namespace glraw
