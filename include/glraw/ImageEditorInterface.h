
#pragma once

#include <glraw/glraw.h>

namespace glraw
{

class ImageEditorInterface
{
public:
    virtual ~ImageEditorInterface() {};

    virtual void editImage(QImage & image, AssetInformation & info) = 0;
};

} // namespace glraw
