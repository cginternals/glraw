
#include "Conversions.h"

#include <QMap>
#include <QString>

#include <glraw/S3TCExtensions.h>

namespace
{

QMap<QString, GLenum> formats()
{
    QMap<QString, GLenum> formats;
    formats["GL_RED"] = GL_RED;
    formats["GL_GREEN"] = GL_GREEN;
    formats["GL_BLUE"] = GL_BLUE;
    formats["GL_RG"] = GL_RG;
    formats["GL_RGB"] = GL_RGB;
    formats["GL_BGR"] = GL_BGR;
    formats["GL_RGBA"] = GL_RGBA;
    formats["GL_BGRA"] = GL_BGRA;

    return formats;
}

QMap<QString, GLenum> types()
{
    QMap<QString, GLenum> types;
    types["GL_UNSIGNED_BYTE"] = GL_UNSIGNED_BYTE;
    types["GL_BYTE"] = GL_BYTE;
    types["GL_UNSIGNED_SHORT"] = GL_UNSIGNED_SHORT;
    types["GL_SHORT"] = GL_SHORT;
    types["GL_UNSIGNED_INT"] = GL_UNSIGNED_INT;
    types["GL_INT"] = GL_INT;
    types["GL_FLOAT"] = GL_FLOAT;

    return types;
}
    
QMap<QString, GLint> compressedFormats()
{
    QMap<QString, GLint> formats;

#ifdef GL_ARB_texture_compression_rgtc
    formats["GL_COMPRESSED_RED_RGTC1"] = GL_COMPRESSED_RED_RGTC1;
    formats["GL_COMPRESSED_SIGNED_RED_RGTC1"] = GL_COMPRESSED_SIGNED_RED_RGTC1;
    formats["GL_COMPRESSED_RG_RGTC2"] = GL_COMPRESSED_RG_RGTC2;
    formats["GL_COMPRESSED_SIGNED_RG_RGTC2"] = GL_COMPRESSED_SIGNED_RG_RGTC2;
#endif
#ifdef GL_ARB_texture_compression_bptc
    formats["GL_COMPRESSED_RGBA_BPTC_UNORM"] = GL_COMPRESSED_RGBA_BPTC_UNORM_ARB;
    formats["GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT"] = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB;
    formats["GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT"] = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB;
#endif
#ifdef GLRAW_DXT // special treatment here - see S3TCExtensions.h
    formats["GL_COMPRESSED_RGB_S3TC_DXT1_EXT"] =  GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
    formats["GL_COMPRESSED_RGBA_S3TC_DXT1_EXT"] =  GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
    formats["GL_COMPRESSED_RGBA_S3TC_DXT3_EXT"] =  GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    formats["GL_COMPRESSED_RGBA_S3TC_DXT5_EXT"] =  GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
#endif
    
    return formats;
}

QMap<QString, Qt::TransformationMode> transformationModes()
{
    QMap<QString, Qt::TransformationMode> modes;
    modes["nearest"] = Qt::FastTransformation;
    modes["linear"] = Qt::SmoothTransformation;

    return modes;
}

QMap<QString, Qt::AspectRatioMode> aspectRatioModes()
{
    QMap<QString, Qt::AspectRatioMode> modes;
    modes["IgnoreAspectRatio"] = Qt::IgnoreAspectRatio;
    modes["KeepAspectRatio"] = Qt::KeepAspectRatio;
    modes["KeepAspectRatioByExpanding"] = Qt::KeepAspectRatioByExpanding;

    return modes;
}

} // namespace

namespace Conversions
{

bool isFormat(const QString & string)
{
    static auto f = formats();

    return f.contains(string);
}

GLenum stringToFormat(const QString & string)
{
    static auto f = formats();

    return f.value(string);
}

bool isType(const QString & string)
{
    static auto t = types();

    return t.contains(string);
}

GLenum stringToType(const QString & string)
{
    static auto t = types();

    return t.value(string);
}
    
bool isCompressedFormat(const QString & string)
{
    static auto f = compressedFormats();
    
    return f.contains(string);
}
    
GLint stringToCompressedFormat(const QString & string)
{
    static auto f = compressedFormats();
    
    return f.value(string);
}

bool isTransformationMode(const QString & string)
{
    static auto m = transformationModes();

    return m.contains(string);
}

Qt::TransformationMode stringToTransformationMode(const QString & string)
{
    static auto m = transformationModes();

    return m.value(string);
}

bool isAspectRatioMode(const QString & string)
{
    static auto m = aspectRatioModes();

    return m.contains(string);
}

Qt::AspectRatioMode stringToAspectRatioMode(const QString & string)
{
    static auto m = aspectRatioModes();

    return m.value(string);
}

} // namespace Conversions
