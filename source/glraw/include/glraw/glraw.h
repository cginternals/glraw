#pragma once

#include <QtGui/qopengl.h>
#include <QString>
#include <QPair>

#include <glraw/glraw_api.h>

namespace glraw
{

enum class TextureType
{
    None,
    Normal,
    Compressed
};

GLRAW_API QString createFilenameSuffix(int width, int height, GLenum format, GLenum type);
GLRAW_API QString createFilenameSuffix(int width, int height, GLenum compressedType);
GLRAW_API TextureType extractFromFilename(const QString & filename, int * width, int * height, GLenum * format, GLenum * type, GLenum * compressedType);

GLRAW_API QString suffixFromFormat(GLenum format);
GLRAW_API QString suffixFromType(GLenum type);
GLRAW_API QString suffixFromCompressedType(GLenum type);

GLRAW_API GLenum formatFromSuffix(const QString & suffix);
GLRAW_API GLenum typeFromSuffix(const QString & suffix);
GLRAW_API GLenum compressedTypeFromSuffix(const QString & suffix);

} // namespace glraw
