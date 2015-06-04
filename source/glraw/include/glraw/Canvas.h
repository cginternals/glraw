#pragma once

#include <functional>

#include <QWindow>
#include <QOpenGLFunctions_3_2_Core>

#include <glraw/glraw_api.h>

class QImage;
class QOpenGLFunctions_3_2_Core;

namespace glraw
{

class AssetInformation;

class GLRAW_API Canvas : public QWindow
{
public:
    Canvas();
    virtual ~Canvas();

	void updateTexture(GLuint tex);
	GLuint texture();
	QOpenGLFunctions_3_2_Core * gl();

    void initializeGL();

	void loadTexture(const QByteArray & image, AssetInformation & info);

    void loadTextureFromImage(const QImage & image);
    QByteArray imageFromTexture(GLenum format, GLenum type);
    QByteArray compressedImageFromTexture(GLenum compressedInternalFormat);


    bool textureLoaded() const;

	void makeContext();
	void doneContext();

protected:
    static int byteSizeOf(GLenum type);
    static int numberOfElementsFor(GLenum format);
    
    QOpenGLContext m_context;
    GLuint m_texture;   

    // using gl as a memeber instead of inheritance 
    // probably resolves an deinitialization issue.
    QOpenGLFunctions_3_2_Core * m_gl;
};

} // namespace glraw
