
#pragma once

namespace glraw
{

enum Format
{
    RAW_GL_RED
,   RAW_GL_RG
,   RAW_GL_RGB
,   RAW_GL_BGR
,   RAW_GL_RGBA
,   RAW_GL_BGRA
,   RAW_GL_STENCIL_INDEX
,   RAW_GL_DEPTH_COMPONENT
,   RAW_GL_DEPTH_STENCIL
};

enum Type
{
    RAW_GL_UNSIGNED_BYTE
,   RAW_GL_BYTE
,   RAW_GL_UNSIGNED_SHORT
,   RAW_GL_SHORT
,   RAW_GL_UNSIGNED_INT
,   RAW_GL_INT
,   RAW_GL_FLOAT
};

} // namespace glraw
