
#include "Conversions.h"

#include <QMap>
#include <QString>

using namespace glraw;

namespace
{

QMap<QString, Format> formats()
{
    QMap<QString, Format> formats;
    formats["GL_RED"] = GL_RED;
    formats["GL_RG"] = GL_RG;
    formats["GL_RGB"] = GL_RGB;
    formats["GL_BGR"] = GL_BGR;
    formats["GL_RGBA"] = GL_RGBA;
    formats["GL_BGRA"] = GL_BGRA;

    return formats;
}

QMap<QString, Type> types()
{
    QMap<QString, Type> types;
    types["GL_UNSIGNED_BYTE"] = GL_UNSIGNED_BYTE;
    types["GL_BYTE"] = GL_BYTE;
    types["GL_UNSIGNED_SHORT"] = GL_UNSIGNED_SHORT;
    types["GL_SHORT"] = GL_SHORT;
    types["GL_UNSIGNED_INT"] = GL_UNSIGNED_INT;
    types["GL_INT"] = GL_INT;
    types["GL_FLOAT"] = GL_FLOAT;

    return types;
}

}

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

} // namespace Conversions
