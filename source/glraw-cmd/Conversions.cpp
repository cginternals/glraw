
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

} // namespace Conversions
