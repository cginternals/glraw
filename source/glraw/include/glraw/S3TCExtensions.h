#pragma once 

#include <QtGui/qopengl.h>

#if defined(WIN32) && !defined(GL_EXT_texture_compression_s3tc)
#define GLRAW_DXT
#elif defined(__APPLE__) && defined(GL_EXT_texture_compression_s3tc)
#define GLRAW_DXT
#elif defined(GL_EXT_texture_compression_s3tc)
#define GLRAW_DXT
#endif
