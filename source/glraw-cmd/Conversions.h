
#pragma once

#include <Qt>
#include <glraw/Enumerations.h>

class QString;

namespace Conversions
{
    bool isFormat(const QString & string);
    glraw::Format stringToFormat(const QString & string);

    bool isType(const QString & string);
    glraw::Type stringToType(const QString & string);

    bool isTransformationMode(const QString & string);
    Qt::TransformationMode stringToTransformationMode(const QString & string);

    bool isAspectRatioMode(const QString & string);
    Qt::AspectRatioMode stringToAspectRatioMode(const QString & string);
}
