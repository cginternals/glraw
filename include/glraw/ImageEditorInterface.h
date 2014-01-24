
#pragma once

#include <glraw/glraw.h>

class QImage;

namespace glraw
{

class AssetInformation;

class GLRAW_API ImageEditorInterface
{
public:
    virtual ~ImageEditorInterface() 
    {
    };

    virtual void editImage(QImage & image, AssetInformation & info) = 0;
};

} // namespace glraw
