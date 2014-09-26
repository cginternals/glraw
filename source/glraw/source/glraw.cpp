#include <glraw/glraw.h>

#include <QMap>
#include <QRegExp>
#include <QStringList>

#include <glraw/S3TCExtensions.h>

namespace
{
    const QMap<GLenum, QString> formatSuffixes = {
        { GL_RED, "r" },
        { GL_GREEN, "g" },
        { GL_BLUE, "b" },
        { GL_RG, "rg" },
        { GL_RGB, "rgb" },
        { GL_BGR, "bgr" },
        { GL_RGBA, "rgba" },
        { GL_BGRA, "bgra" }
    };
    const QMap<QString, GLenum> fromFormatSuffixes = {
        { "r", GL_RED },
        { "g", GL_GREEN },
        { "b", GL_BLUE },
        { "rg", GL_RG },
        { "rgb", GL_RGB },
        { "bgr", GL_BGR },
        { "rgba", GL_RGBA },
        { "bgra", GL_BGRA }
    };
    const QMap<GLenum, QString> typeSuffixes = {
        { GL_UNSIGNED_BYTE, "ub" },
        { GL_BYTE, "b" },
        { GL_UNSIGNED_SHORT, "us" },
        { GL_SHORT, "s" },
        { GL_UNSIGNED_INT, "ui" },
        { GL_INT, "i" },
        { GL_FLOAT, "f" }
    };
    const QMap<QString, GLenum> fromTypeSuffixes = {
        { "ub", GL_UNSIGNED_BYTE },
        { "b", GL_BYTE },
        { "us", GL_UNSIGNED_SHORT },
        { "s", GL_SHORT },
        { "ui", GL_UNSIGNED_INT },
        { "i", GL_INT },
        { "f", GL_FLOAT }
    };
    const QMap<GLint, QString> compressedFormatSuffixes = {
    #ifdef GL_ARB_texture_compression_rgtc
          { GL_COMPRESSED_RED_RGTC1, "rgtc1-r" }
        , { GL_COMPRESSED_SIGNED_RED_RGTC1, "rgtc1-sr" }
        , { GL_COMPRESSED_RG_RGTC2, "rgtc2-rg" }
        , { GL_COMPRESSED_SIGNED_RG_RGTC2, "rgtc2-srg" }
    #endif
    #ifdef GL_ARB_texture_compression_bptc
    #ifdef GL_ARB_texture_compression_rgtc
        ,
    #endif
          { GL_COMPRESSED_RGBA_BPTC_UNORM_ARB, "bptc-rgba-unorm" }
        , { GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB, "bptc-rgb-sf" }
        , { GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB, "bptc-rgb-uf" }
    #endif
    #ifdef GLRAW_DXT // special treatment here - see S3TCExtensions.h
    #if defined(GL_ARB_texture_compression_rgtc) || defined(GL_ARB_texture_compression_bptc)
        ,
    #endif
          { GL_COMPRESSED_RGB_S3TC_DXT1_EXT, "dxt1-rgb" }
        , { GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, "dxt1-rgba" }
        , { GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, "dxt3-rgba" }
        , { GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, "dxt5-rgba" }
    #endif
    };
    const QMap<QString, GLenum> fromCompressedFormatSuffixes = {
    #ifdef GL_ARB_texture_compression_rgtc
          { "rgtc1-r", GL_COMPRESSED_RED_RGTC1 }
        , { "rgtc1-sr", GL_COMPRESSED_SIGNED_RED_RGTC1 }
        , { "rgtc2-rg", GL_COMPRESSED_RG_RGTC2 }
        , { "rgtc2-srg", GL_COMPRESSED_SIGNED_RG_RGTC2 }
    #endif
    #ifdef GL_ARB_texture_compression_bptc
    #ifdef GL_ARB_texture_compression_rgtc
        ,
    #endif
          { "bptc-rgba-unorm", GL_COMPRESSED_RGBA_BPTC_UNORM_ARB }
        , { "bptc-rgb-sf", GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB }
        , { "bptc-rgb-uf", GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB }
    #endif
    #ifdef GLRAW_DXT // special treatment here - see S3TCExtensions.h
    #if defined(GL_ARB_texture_compression_rgtc) || defined(GL_ARB_texture_compression_bptc)
        ,
    #endif
          { "dxt1-rgb", GL_COMPRESSED_RGB_S3TC_DXT1_EXT }
        , { "dxt1-rgba", GL_COMPRESSED_RGBA_S3TC_DXT1_EXT }
        , { "dxt3-rgba", GL_COMPRESSED_RGBA_S3TC_DXT3_EXT }
        , { "dxt5-rgba", GL_COMPRESSED_RGBA_S3TC_DXT5_EXT }
    #endif
    };
}

namespace glraw
{

QString createFilenameSuffix(int width, int height, GLenum format, GLenum type)
{
    return QString(".%1.%2.%3.%4")
        .arg(width)
        .arg(height)
        .arg(suffixFromFormat(format))
        .arg(suffixFromType(type));
}

QString createFilenameSuffix(int width, int height, GLenum compressedType)
{
    return QString(".%1.%2.%3")
        .arg(width)
        .arg(height)
        .arg(suffixFromCompressedType(compressedType));
}

TextureType extractFromFilename(const QString & filename, int * width, int * height, GLenum * format, GLenum * type, GLenum * compressedType)
{
    QRegExp normalRegExp(R"(^.*\.(\d+)\.(\d+)\.(\w+)\.(\w+)\.raw$)");
    QRegExp compressedRegExp(R"(^.*\.(\d+)\.(\d+)\.(\w+)\.raw$)");

    if (normalRegExp.exactMatch(filename))
    {
        QStringList parts = normalRegExp.capturedTexts();

        *width = parts[1].toInt();
        *height = parts[2].toInt();
        *format = glraw::formatFromSuffix(parts[3].toLower());
        *type = glraw::typeFromSuffix(parts[4].toLower());

        return TextureType::Normal;
    }
    else if (compressedRegExp.exactMatch(filename))
    {
        QStringList parts = normalRegExp.capturedTexts();

        *width = parts[1].toInt();
        *height = parts[2].toInt();

        *compressedType = glraw::compressedTypeFromSuffix(parts[3].toLower());

        return TextureType::Compressed;
    }

    return TextureType::None;
}

QString suffixFromFormat(GLenum format)
{
    return formatSuffixes.value(format, "");
}

QString suffixFromType(GLenum type)
{
    return typeSuffixes.value(type, "");
}

QString suffixFromCompressedType(GLenum type)
{
    return compressedFormatSuffixes.value(type, "");
}

GLenum formatFromSuffix(const QString & suffix)
{
    return fromFormatSuffixes.value(suffix.toLower(), 0);
}

GLenum typeFromSuffix(const QString & suffix)
{
    return fromTypeSuffixes.value(suffix.toLower(), 0);
}

GLenum compressedTypeFromSuffix(const QString & suffix)
{
    return fromCompressedFormatSuffixes.value(suffix.toLower(), 0);
}

} // namespace glraw
