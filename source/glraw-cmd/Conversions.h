
#pragma once

#include <glraw/Enumerations.h>

class QString;

namespace Conversions
{
    bool isFormat(const QString & string);
    glraw::Format stringToFormat(const QString & string);
}
