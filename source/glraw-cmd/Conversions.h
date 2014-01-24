
#pragma once

#include <Qt>
#include <QtGui/qopengl.h>

class QString;

namespace Conversions
{
    bool isFormat(const QString & string);
    GLenum stringToFormat(const QString & string);

    bool isType(const QString & string);
    GLenum stringToType(const QString & string);
    
    bool isCompressedFormat(const QString & string);
    GLint stringToCompressedFormat(const QString & string);

    bool isTransformationMode(const QString & string);
    Qt::TransformationMode stringToTransformationMode(const QString & string);

    bool isAspectRatioMode(const QString & string);
    Qt::AspectRatioMode stringToAspectRatioMode(const QString & string);
}
