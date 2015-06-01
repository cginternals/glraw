#pragma once

#include <glraw/glraw_api.h>

#include <QVariantMap>

namespace glraw
{

class AssetInformation;
class Canvas;

class GLRAW_API AbstractFilter
{
public:
    AbstractFilter() = default;
    virtual ~AbstractFilter() = default;

    virtual bool process(Canvas & imageData, AssetInformation & info) = 0;
};

} // namespace glraw
