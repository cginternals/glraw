
#include "Conversions.h"

#include <QMap>
#include <QString>

using namespace glraw;

namespace
{

QMap<QString, Format> formats()
{
    QMap<QString, Format> formats;
    formats["GL_RED"] = RAW_GL_RED;
    formats["GL_RG"] = RAW_GL_RG;
    formats["GL_RGB"] = RAW_GL_RGB;
    formats["GL_BGR"] = RAW_GL_BGR;
    formats["GL_RGBA"] = RAW_GL_RGBA;
    formats["GL_BGRA"] = RAW_GL_BGRA;

    return formats;
}

QMap<QString, Type> types()
{
    QMap<QString, Type> types;
    types["GL_UNSIGNED_BYTE"] = RAW_GL_UNSIGNED_BYTE;
    types["GL_BYTE"] = RAW_GL_BYTE;
    types["GL_UNSIGNED_SHORT"] = RAW_GL_UNSIGNED_SHORT;
    types["GL_SHORT"] = RAW_GL_SHORT;
    types["GL_UNSIGNED_INT"] = RAW_GL_UNSIGNED_INT;
    types["GL_INT"] = RAW_GL_INT;
    types["GL_FLOAT"] = RAW_GL_FLOAT;

    return types;
}

QMap<QString, Qt::TransformationMode> transformationModes()
{
    QMap<QString, Qt::TransformationMode> modes;
    modes["nearest"] = Qt::FastTransformation;
    modes["bilinear"] = Qt::SmoothTransformation;

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

Format stringToFormat(const QString & string)
{
    static auto f = formats();

    return f.value(string);
}

bool isType(const QString & string)
{
    static auto t = types();

    return t.contains(string);
}

glraw::Type stringToType(const QString & string)
{
    static auto t = types();

    return t.value(string);
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
